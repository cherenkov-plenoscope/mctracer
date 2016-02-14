//=================================
// include guard
#ifndef __FunctionFab_H_INCLUDED__
#define __FunctionFab_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Function/Func1DFunction.h"
#include "Core/Function/LimitsFunction.h"
#include "Xml/Xml.h"
#include "Tools/PathTools.h"
#include <map>
//------------------------------------------------------------------------------
class FunctionFab {

	PathTools::FullPath full_path_of_original_xml_file;
	std::map<std::string, const Function::Func1D*> functions;
public:

	void add_function_from(const Xml::Node &node);
	const Function::Func1D* get_function_by_name(const std::string name)const;
private:
	bool is_function_node(const Xml::Node &node)const;
	std::string extract_name(const Xml::Node &node);
	bool has_function(const std::string key)const;
	void assert_name_not_in_use_yet(const std::string key)const;
	void assert_is_function_node(const Xml::Node &node)const;
	Function::Func1D* extract_linear_interpolation(const Xml::Node &node);
	Function::Func1D* extract_constant(const Xml::Node &node);
	Function::Func1D* extract_polynom3(const Xml::Node &node);
	Function::Func1D* extract_concatenation(const Xml::Node &node); 
	Function::Limits extract_limits_from_attributes(const Xml::Node &node);
	void assert_has_function(const std::string name)const;
	void export_function_based_on_node(const Function::Func1D* func, const Xml::Node &node); 
};
#endif // __FunctionFab_H_INCLUDED__ 