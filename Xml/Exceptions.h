// Copyright 2014 Sebastian A. Mueller
#ifndef XML_EXCEPTIONS_H_
#define XML_EXCEPTIONS_H_

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

}  // namespace Xml

#endif  // XML_EXCEPTIONS_H_
