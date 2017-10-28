//=================================
// include guard
#ifndef __XmlExceptions_H_INCLUDED__
#define __XmlExceptions_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies

namespace Xml {

	class IoError : public std::runtime_error {
		using runtime_error::runtime_error;
	};

	class SyntaxError : public std::invalid_argument {
		using invalid_argument::invalid_argument;
	};

	class InternalError : public std::runtime_error {
		using runtime_error::runtime_error;
	};

	class NoSuchChild : public std::invalid_argument {
		using invalid_argument::invalid_argument;
	};

	class NoSuchAttribute : public std::invalid_argument {
		using invalid_argument::invalid_argument;
	};

	class BadAttribute : public std::invalid_argument {
		using invalid_argument::invalid_argument;
	};
}//Xml

#endif // __XmlExceptions_H_INCLUDED__ 
