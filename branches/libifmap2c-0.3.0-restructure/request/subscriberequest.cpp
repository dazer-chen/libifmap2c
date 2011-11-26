/*
 * COPYRIGHT AND PERMISSION NOTICE
 * 
 * Copyright (c) 2010-2011, Arne Welzel, <arne.welzel@googlemail.com>
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

#include "subscriberequest.h"
#include "typedefs.h"

using namespace std;

namespace ifmap2c {

SubscribeRequest::SubscribeRequest(const list<SubscribeElement *>& list) :
	_subscribeElements(list)
{ }
SubscribeRequest::~SubscribeRequest()
{
	CSELISTIT it, end;
	it = _subscribeElements.begin();
	end = _subscribeElements.end();

	for (/* */; it != end; it++)
		delete *it;
}

SubscribeRequest *
SubscribeRequest::createSubscribeRequest(const std::list<SubscribeElement *>& subList)
{
	return new SubscribeRequest(subList);

}

SubscribeRequest *
SubscribeRequest::createSubscribeRequest(SubscribeElement *const subreq)
{
	list<SubscribeElement *> subList;
	subList.push_back(subreq);
	return createSubscribeRequest(subList);
}
const std::list<SubscribeElement *>&
SubscribeRequest::getSubscribeElements(void) const
{
	return _subscribeElements;
}

} // namespace
