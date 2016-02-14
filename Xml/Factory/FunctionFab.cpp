#include "FunctionFab.h"
#include "Tools/StringTools.h"
#include "Tools/AsciiIo.h"
#include "Core/Function/Function.h"

//------------------------------------------------------------------------------
void FunctionFab::add_function_from(const Xml::Node &node) {

	full_path_of_original_xml_file = 
		PathTools::split_path_and_filename(node.get_xml_path());

	const Function::Func1D* func = &Function::Constant::void_function;
	bool export_function = false;

	for(
		Xml::Node child = node.get_first_child(); 
		child; 
		child = child.get_next_child()
	){	
		if 		(StringTools::is_equal(child.get_name(),"linear_interpolation")){
			func = extract_linear_interpolation(child);
		}else if(StringTools::is_equal(child.get_name(),"constant")){
			func = extract_constant(child);
		}else if(StringTools::is_equal(child.get_name(),"polynom3")){
			func = extract_polynom3(child);
		}else if(StringTools::is_equal(child.get_name(),"concatenation")){
			func = extract_concatenation(child);
		}else if(StringTools::is_equal(child.get_name(),"export")){
			export_function = true;
		}else{
			std::stringstream info;
			info << "FunctionFab " << __FILE__ << ", " << __LINE__;
			info << "Unknown xml child in '" << child.get_name() << "' in node ";
			info << "'" << node.get_name() << "'";
			throw TracerException(info.str());
		}
	}

	assert_name_not_in_use_yet(node.get_attribute("name"));

	functions.insert(
		std::pair<std::string, const Function::Func1D*>
			(node.get_attribute("name"), func)
	);

	if(export_function)
		export_function_based_on_node(func, node.get_child("export"));

}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void FunctionFab::export_function_based_on_node(
	const Function::Func1D* func,
	const Xml::Node &node
) {
	AsciiIo::write_table_to_file(
		func->get_samples(Xml::attribute_to_int(node, "samples")),
		full_path_of_original_xml_file.path + node.get_attribute("file")
	);
}
//------------------------------------------------------------------------------
bool FunctionFab::has_function(const std::string key)const {
	return functions.find(key) != functions.end();
}
//------------------------------------------------------------------------------
void FunctionFab::assert_name_not_in_use_yet(const std::string key)const {
	if(has_function(key)) {
		std::stringstream info;
		info << "FunctionFab " << __FILE__ << ", " << __LINE__ << "\n";
		info << "| redecleration of function " << key <<"\n";
		throw TracerException(info.str());
	}
}
//------------------------------------------------------------------------------
void FunctionFab::assert_is_function_node(const Xml::Node &node)const {
	if(!is_function_node(node)) {
		std::stringstream info;
		info << "FunctionFab " << __FILE__ << ", " << __LINE__ << "\n";
		info << "Expected node name to be: function, but actually: ";
		info << node.get_name() <<"\n";
		throw TracerException(info.str());
	}
}
//------------------------------------------------------------------------------
bool FunctionFab::is_function_node(const Xml::Node &node)const {
	return StringTools::is_equal(node.get_name(),"function");
}
//------------------------------------------------------------------------------
Function::Func1D* FunctionFab::extract_linear_interpolation(const Xml::Node &node) {
	std::vector<std::vector<double>> table = AsciiIo::gen_table_from_file(
		full_path_of_original_xml_file.path + node.get_attribute("file")
	);
	return new Function::LinInterpol(table);
}
//------------------------------------------------------------------------------
Function::Func1D* FunctionFab::extract_constant(const Xml::Node &node) {
	return new Function::Constant(
		Xml::attribute_to_double(node, "value"),
		extract_limits_from_attributes(node)
	);
}
//------------------------------------------------------------------------------
Function::Func1D* FunctionFab::extract_polynom3(const Xml::Node &node) {
	return new Function::Polynom3(
		Xml::attribute_to_double(node, "x3"),
		Xml::attribute_to_double(node, "x2"),
		Xml::attribute_to_double(node, "x1"),
		Xml::attribute_to_double(node, "x0"),
		extract_limits_from_attributes(node)
	);
}
//------------------------------------------------------------------------------
Function::Func1D* FunctionFab::extract_concatenation(const Xml::Node &node) {
	std::string attribute = "f0";
	std::vector<const Function::Func1D*> funcs;
	uint count = 0;

	while (node.has_attribute(attribute)) {

		funcs.push_back(get_function_by_name(node.get_attribute(attribute)));
		count++;
		std::stringstream satt; satt << "f" << count;
		attribute = satt.str();
	};

	return new Function::Concat(funcs);
}
//------------------------------------------------------------------------------
Function::Limits FunctionFab::extract_limits_from_attributes(const Xml::Node &node) {
	return Function::Limits(
		Xml::attribute_to_double(node, "lower_limit"), 
		Xml::attribute_to_double(node, "upper_limit")
	);
}
//------------------------------------------------------------------------------
const Function::Func1D* FunctionFab::get_function_by_name(const std::string name)const {
	assert_has_function(name);
	return functions.find(name)->second;
}
//------------------------------------------------------------------------------
void FunctionFab::assert_has_function(const std::string name)const {
	if(!has_function(name)) {
		std::stringstream info;
		info << "FunctionFab " << __FILE__ << ", " << __LINE__ << "\n";
		info << "There is no function called '" << name << "'. \n";
		throw TracerException(info.str());
	}
}