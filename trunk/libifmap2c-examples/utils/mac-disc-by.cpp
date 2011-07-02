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
 */

#include <iostream>

// libifmap2c includes
#include <libifmap2c/ssrc.h>
#include <libifmap2c/identifiers.h>
#include <libifmap2c/metadata.h>

#include "common.h"

// make life easier
using namespace ifmap2c;
using namespace std;

static void usage(const char *prog)
{
	cerr << "usage: " << prog << " update|delete mac dev"
		INDEPENDENT_USAGE_STRING << endl;
}

int main(int argc, char* argv[])
{
	char *devArg, *macArg, *op;
	char *url, *user, *pass, *capath;
	url = user = pass = capath = NULL;
	SSRC *ssrc = NULL;
	PublishRequest *pubReq = NULL;
	PublishElement *subReq = NULL;
	XmlMarshalable *discby = NULL;
	Identifier *dev, *mac;

	checkAndLoadParameters(argc, argv, 3, usage, &url, &user,
			&pass, &capath);

	op = argv[1];
	macArg = argv[2];
	devArg = argv[3];

	checkUpdateOrDelete(op, usage, argv[0]);
	
	ssrc = SSRC::createSSRC(url, user, pass, capath);
	mac = Identifiers::createMac(macArg);
	dev = Identifiers::createDev(devArg);

	if (isUpdate(op)) {
		discby = Metadata::createDiscoveredBy();
		subReq = Requests::createPublishUpdate(discby, mac, dev,
				forever);
	} else {
		subReq = Requests::createPublishDelete(
				"meta:discovered-by", mac, dev);
	}

	pubReq = Requests::createPublishReq(subReq);
	pubReq->addXmlNamespaceDefinition(TCG_META_NSPAIR);

	try {
		ssrc->newSession();
		ssrc->publish(pubReq);
		ssrc->endSession();
	} catch (IfmapError e) {
		cerr << e << endl;
	} catch (ErrorResultError e) {
		cerr << e << endl;
	}
	
	delete pubReq;
	delete ssrc;

	return 0;
}
