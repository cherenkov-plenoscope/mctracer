//=================================
// include guard
#ifndef __XmlExceptions_H_INCLUDED__
#define __XmlExceptions_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "TracerException.h"

namespace Xml {

	class IoError : public TracerException {
		using TracerException::TracerException;
	};

	class SyntaxError : public TracerException {
		using TracerException::TracerException;
	};

	class InternalError : public TracerException {
		using TracerException::TracerException;
	};

	class NoSuchChild : public TracerException {
		using TracerException::TracerException;
	};

	class NoSuchAttribute : public TracerException {
		using TracerException::TracerException;
	};

	class BadAttribute : public TracerException {
		using TracerException::TracerException;
	};
}//Xml

#endif // __XmlExceptions_H_INCLUDED__ 
