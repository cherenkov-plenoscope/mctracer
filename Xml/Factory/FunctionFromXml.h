//=================================
// include guard
#ifndef __FunctionFromXml_H_INCLUDED__
#define __FunctionFromXml_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Function/Function.h"
#include "Xml/Xml.h"

//------------------------------------------------------------------------------
namespace Xml {
	Function::LinInterpol get_LinInterpol_from(const Xml::Node &node);
}
#endif // __FunctionFromXml_H_INCLUDED__ 