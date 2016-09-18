//=================================
// include guard
#ifndef __XmlFunction_H_INCLUDED__
#define __XmlFunction_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Function/Function.h"
#include "Xml/Xml.h"
#include "Scenery/FunctionMap.h"

//------------------------------------------------------------------------------
namespace Xml {
	void init_function_with_node(Function::Constant* function, const Node &node);
	void init_function_with_node(Function::Polynom3* function, const Node &node);
	void init_function_with_node(Function::LinInterpol* function, const Node &node);
	void init_function_with_node(Function::Concat* function, const Node &node, FunctionMap* functions);
	Function::Limits limits_from_node(const Xml::Node &node);
}//Xml

#endif // __XmlFunction_H_INCLUDED__ 