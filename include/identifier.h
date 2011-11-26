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

#ifndef IDENTIFIER_H_
#define IDENTIFIER_H_
#include "xmlmarshalable.h"
#include "ifmaperror.h"
#include <typeinfo>
#include <list>

namespace ifmap2c {

class Identifier {

protected:
	Identifier();

public:
	virtual ~Identifier();
};

class BadIdentifier : public IfmapError {
public:
	BadIdentifier(const std::string& msg) 
		: IfmapError("BadIdentifier", msg)
	{ };
};

class IdentifierAdmin : public Identifier {
public:
	virtual ~IdentifierAdmin();
	const std::string& getAdministrativeDomain(void) const;

protected:
	IdentifierAdmin(const std::string& ad);

private:
	std::string _administrativeDomain;
};

class IdentifierHolder {

public:
	virtual ~IdentifierHolder() {
		if (_i1)
			delete _i1;

		if (_i2)
			delete _i2;
	};

	Identifier *getIdentifier1(void) const {
		return _i1;
	}
	
	Identifier *getIdentifier2(void) const {
		return _i2;
	}

protected:
	IdentifierHolder(Identifier *const i1,
			Identifier *const i2 = NULL) :
		_i1(i1), _i2(i2) { }

private:
	Identifier *const _i1;
	Identifier *const _i2;
};

class IdentifierMetadataHolder : public IdentifierHolder {

public:
	virtual ~IdentifierMetadataHolder() {
		std::list<XmlMarshalable *>::const_iterator it, end;
		it = _metadata.begin();
		end = _metadata.end();

		for (/* */; it != end; it++)
			delete *it;
	};

	const std::list<XmlMarshalable *>& getMetadata(void) const {
		return _metadata;
	}

protected:
	IdentifierMetadataHolder(Identifier *const i1,
			Identifier *const i2,
			std::list<XmlMarshalable *> mList) :
		IdentifierHolder(i1, i2), _metadata(mList) { };

private:
	const std::list<XmlMarshalable *> _metadata;
};


/**
 * Interface for IdentifierHandler
 */
class IdentifierHandler {

public:
	virtual XmlMarshalable *toXml(Identifier *const ident) = 0;
	virtual Identifier *fromXml(XmlMarshalable *const xml) = 0;
	virtual bool canHandle(Identifier * const parname) const = 0;
};


class IfmapIdentifierHandlerDispatch {

public:
	/**
	 * Register a new IdentifierHandler
	 */
	static void
	registerHandler(IdentifierHandler *const handler);
	
	/**
	 * Deregister and free all registered handlers to keep
	 * valgrind happy
	 */
	static void
	clearHandlers(void);

	/**
	 * Dispatch XML generation to an appropiate IdentifierHandler.
	 *
	 * Find the responsible IdentifierHandler and delegate
	 * the call accordingly to generate something we can make
	 * XML out of
	 */
	XmlMarshalable *toXml(Identifier *const i) const;

	/**
	 * Try each registered IdentifierHandler to create an
	 * Identifier from the given XML
	 */
	Identifier *fromXml(XmlMarshalable *const xml) const;

private:
	static std::list<IdentifierHandler *> handlers;
};

} // namespace
#endif /* IDENTIFIER_H_ */
