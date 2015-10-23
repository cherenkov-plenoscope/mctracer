//=================================
// include guard
#ifndef __FUNCTIONFACTORY_H_INCLUDED__
#define __FUNCTIONFACTORY_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Function/Func1DFunction.h"
#include "Core/Function/LimitsFunction.h"
#include "XmlIO/XmlFileIo.h"
#include <map>
//------------------------------------------------------------------------------
class FunctionFactory : public XmlFileIo{

	const XmlFileIo* mother_factory;
	std::map<std::string, const Function::Func1D*> functions;
public:
	FunctionFactory(const XmlFileIo* io_to_take_over);
	void extract_function_from(const pugi::xml_node node);
	const Function::Func1D* get_function(const std::string name)const;
private:
	bool is_function_node(const pugi::xml_node node)const;
	std::string extract_name(const pugi::xml_node node);
	bool has_function(const std::string key)const;
	void assert_name_is_unique(const std::string key)const;
	void assert_is_function_node(const pugi::xml_node node)const;
	Function::Func1D* extract_linear_interpolation(const pugi::xml_node node);
	Function::Func1D* extract_constant(const pugi::xml_node node);
	Function::Func1D* extract_polynom3(const pugi::xml_node node);
	Function::Func1D* extract_concatenation(const pugi::xml_node node); 
	Function::Limits extract_limits_from_attributes(const pugi::xml_node node);
	void assert_has_function(const std::string name)const;
	void export_function_based_on_node(
		const Function::Func1D* func,
		const pugi::xml_node node
	); 
};
#endif // __FUNCTIONFACTORY_H_INCLUDED__ 