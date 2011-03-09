/*
 * COPYRIGHT AND PERMISSION NOTICE
 *
 * Copyright (c) 2011, Arne Welzel, <arne.welzel@googlemail.com>
 *
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software for any purpose
 * with or without fee is hereby granted, provided that the above copyright
 * notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD PARTY RIGHTS. IN
 * NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
 * OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of a copyright holder shall not
 * be used in advertising or otherwise to promote the sale, use or other dealings
 * in this Software without prior written authorization of the copyright holder.
 *
 * There was a bug reported in 0.2.0, reproduce it...
 */


// libifmap2c includes
#include <libifmap2c/requests.h>
#include <libifmap2c/ssrc.h>
#include <libifmap2c/arc.h>
#include <libifmap2c/identifiers.h>
#include <libifmap2c/metadata.h>

#include <iostream>
#include <cstdlib>
#include <list>


using namespace std;
using namespace ifmap2c;

typedef pair<string, string> STRP;

static STRP mdNs("myns", "http://mynamespace.com");

class SimpleEvent : public BasicXmlMarshalable {
public:
	SimpleEvent(const string& info) :
		BasicXmlMarshalable("simpleEvent", "", mdNs)
	{
		addXmlAttribute(STRP(META_CARDINALITY_ATTR_NAME, "singleValue"));
		addXmlAttribute(STRP("event-info", info));
		addXmlNamespaceDefinition(mdNs);
	}
};


static void
checkOnlyIp(PollResult *const pollres, IpAddress *const ip)
{
	// we only expect one search result with one element for
	// subscription sub1
	list<SearchResult *> sresults = pollres->getSearchResults();
	list<SearchResult *> uresults = pollres->getUpdateResults();
	list<SearchResult *> dresults = pollres->getDeleteResults();
	list<SearchResult *> nresults = pollres->getNotifyResults();
	if (sresults.size() != 1) {
		cout << " [ERROR: Wrong number of SearchResults] ";
		return;
	}
	
	if ((uresults.size() != 0) || (dresults.size() != 0) ||
			(nresults.size() != 0)) {
		cout << " [ERROR: Other things than the SearchResult] "; 
		return;
	}

	SearchResult *sres = *sresults.begin();
	if (sres->getResultItems().size() != 1) {
		cout << " [ERROR: Wrong number of ResultItems] ";
		return;
	}

	ResultItem *ri = sres->getResultItem(ip);
	if (ri) {
		if (ri->getMetadataList().size() != 0) {
			cout << " [ERROR: Metadata for IP?!] ";
		}
	} else {
		cout << "[ERROR: No ResultItem for the IP] "; 
		return;
	}
	cout << " [SearchResult is good] ";
	return;
}

static void
checkForTwoEvents(PollResult *const pollres, IpAddress *const ip)
{
	// we only expect one notify result filled with one result
	// item containing two simpleEvents
	list<SearchResult *> sresults = pollres->getSearchResults();
	list<SearchResult *> uresults = pollres->getUpdateResults();
	list<SearchResult *> dresults = pollres->getDeleteResults();
	list<SearchResult *> nresults = pollres->getNotifyResults();
	
	if (nresults.size() != 1) {
		cout << " [ERROR: Wrong number of SearchResults] ";
		return;
	}
	
	if ((sresults.size() != 0) || (uresults.size() != 0) ||
			(dresults.size() != 0)) {
		cout << " [ERROR: Other things than the SearchResult] "; 
		return;
	}

	SearchResult *nres = *nresults.begin();
	if (nres->getResultItems().size() != 1) {
		cout << " [ERROR: Wrong number of ResultItems] ";
		return;
	}

	ResultItem *ri = nres->getResultItem(ip);
	if (ri) {
		if (ri->getMetadataList().size() != 2) {
			cout << " [ERROR: Wrong Metadata count for IP] ";
			return;
		} else {
			list<XmlMarshalable *> found =
				XmlMarshalable::findMatchingElements(
				ri->getMetadataList(),
				"simpleEvent",
				"http://mynamespace.com");
			if (found.size() != 2) {
				cout << " [ERROR: Not the events!] ";
				return;
			}
		}
	} else {
		cout << " [ERROR: No ResultItem for the IP] ";
		return;
	}
	cout << " [NotifyResult is good] ";
	return;
}

static void
usage(const char *const name)
{
	cerr << "Usage: " << name << " ifmap-server-url username password capath" << endl;
	exit(1);
}

int
main(int argc, char *argv[])
{
	if (argc != 5)
		usage(argv[0]);

	// create ssrc object which is used for synchronous communication
	SSRC *ssrc = SSRC::createSSRC(argv[1], argv[2], argv[3],argv[4]);
	ARC *arc = ssrc->getARC();

	// prepare a publish request to ip-address identifier with
	// two events
	IpAddress *ip = Identifiers::createIPv4("192.168.1.11");
	list<XmlMarshalable *> events;
	events.push_back(new SimpleEvent("event1"));
	events.push_back(new SimpleEvent("event2"));
	SubPublish *up1 = Requests::createPublishNotify(events, ip->clone());
	PublishRequest *pr1 = Requests::createPublishReq(up1);
	
	

	list<SubPublish *> uplist;
	uplist.push_back(Requests::createPublishNotify(new SimpleEvent(
					"event1"), ip->clone()));
	uplist.push_back(Requests::createPublishNotify(new SimpleEvent(
				"event1"), ip->clone()));
	PublishRequest *pr2 = Requests::createPublishReq(uplist);

	SubSubscribe *subcreate = Requests::createSubscribeUpdate(
			"sub1",
			FILTER_MATCH_ALL,
			0,
			FILTER_MATCH_ALL,
			2000,
			ip);


	SubscribeRequest *subreq = Requests::createSubscribeReq(subcreate);

	try {
		cout << "Doing newSession\t";
		ssrc->newSession();
		cout << "Ok" << endl;
		cout << "Doing subscribe\t\t";
		ssrc->subscribe(subreq);
		cout << "Ok" << endl;
		cout << "Doing first poll\t";
		PollResult *pollres = arc->poll();
		checkOnlyIp(pollres, ip);
		delete pollres;
		cout << "Ok" << endl;
		cout << "Doing publish 1\t\t";
		ssrc->publish(pr1);
		cout << "Ok" << endl;
		cout << "Doing second poll\t";
		pollres = arc->poll();
		checkForTwoEvents(pollres, ip);
		delete pollres;
		cout << "Ok" << endl;
		
		cout << "Doing publish 2\t\t";
		ssrc->publish(pr2);
		cout << "Ok" << endl;
		
		cout << "Doing third poll\t";
		pollres = arc->poll();
		checkForTwoEvents(pollres, ip);
		delete pollres;
		cout << "Ok" << endl;
		cout << "Doing endSession\t";
		ssrc->endSession();
		cout << "Ok" << endl;

	} catch (CommunicationError e) {
		cerr << "CommunicationError: " << e.getMessage() << endl;
	} catch (ErrorResultError e) {
		cerr << "ErrorResult:" << endl;
		cerr << " " << e.getErrorCodeString() << endl;
		cerr << " " << e.getErrorString() << endl;
	} catch (XmlMarshalError e) {
		cerr << "XmlMarshalError: " << e.getMessage() << endl;
	} catch (ResponseParseError e) {
		cerr << "ResponseParseError: " << e.getMessage() << endl;
	} catch (...) {
		cerr << "Uncatched Exception occured" << endl;
		throw;
	}

	delete pr1;
	delete pr2;
	delete subreq;
	
	// this closes the TCP connections
	delete arc;
	delete ssrc;
	return 0;
}
