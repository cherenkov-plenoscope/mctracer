// Copyright 2014 Sebastian A. Mueller
#ifndef XML_FACTORY_FUNCTIONFROMXML_H_
#define XML_FACTORY_FUNCTIONFROMXML_H_

#include "Core/Function/Function.h"
#include "Xml/Xml.h"

namespace Xml {
    Function::LinInterpol get_LinInterpol_from(const Xml::Node &node);
}  // namespace Xml

#endif  // XML_FACTORY_FUNCTIONFROMXML_H_
