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
#include <cstdlib>
#include <cstring>

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
	cerr << "usage: " << prog << " update|delete device ip"
		INDEPENDENT_USAGE_STRING << endl;
	exit(1);
}

int main(int argc, char* argv[])
{
	char *devArg, *ipArg, *op;
	char *url, *user, *pass, *capath;
	url = user = pass = capath = NULL;
	SSRC *ssrc = NULL;
	PublishRequest *pubReq = NULL;
	SubPublish *subReq = NULL;
	XmlMarshalable *devip = NULL;
	Identifier *dev, *ip;

	checkAndLoadParameters(argc, argv, 4, usage, &url, &user,
			&pass, &capath);

	op = argv[1];
	devArg = argv[2];
	ipArg = argv[3];

	checkUpdateOrDelete(op, usage, argv[0]);
	
	ssrc = SSRC::createSSRC(url, user, pass, capath);
	dev = Identifiers::createDev(devArg);
	ip = Identifiers::createIPv4(ipArg);

	if (isUpdate(op)) {
		devip = Metadata::createDevIp();
		subReq = Requests::createPublishUpdate(devip, dev,
				forever, ip);
	} else {
		subReq = Requests::createPublishDelete(
				"meta:device-ip",
				dev, ip);
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