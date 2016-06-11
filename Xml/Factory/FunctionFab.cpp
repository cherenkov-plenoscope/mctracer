#include "FunctionFab.h"
#include "Tools/StringTools.h"
#include "Tools/AsciiIo.h"
#include "Core/Function/Function.h"
using std::stringstream;

using StringTools::is_equal;
//------------------------------------------------------------------------------
void FunctionFab::add(const Xml::Node &node) {

	full_path_of_original_xml_file = 
		PathTools::Path(node.xml_path());

	const Function::Func1D* func = &Function::Constant::void_function;
	bool export_function = false;

	for(Xml::Node child=node.first_child();	child; child=child.next_child()) {

		if 		(is_equal(child.name(),"linear_interpolation")){
			func = extract_linear_interpolation(child);
		}else if(is_equal(child.name(),"constant")){
			func = extract_constant(child);
		}else if(is_equal(child.name(),"polynom3")){
			func = extract_polynom3(child);
		}else if(is_equal(child.name(),"concatenation")){
			func = extract_concatenation(child);
		}else if(is_equal(child.name(),"export")){
			export_function = true;
		}else{
			stringstream info;
			info << "FunctionFab " << __FILE__ << ", " << __LINE__;
			info << "Unknown xml child in '" << child.name() << "' in node ";
			info << "'" << node.name() << "'";
			throw TracerException(info.str());
		}
	}

	assert_name_not_in_use_yet(node.attribute("name"));

	functions.insert(
		std::pair<string, const Function::Func1D*>
			(node.attribute("name"), func)
	);

	if(export_function)
		export_function_based_on_node(func, node.child("export"));
}
//------------------------------------------------------------------------------
void FunctionFab::export_function_based_on_node(
	const Function::Func1D* func,
	const Xml::Node &node
) {
	AsciiIo::write_table_to_file(
		func->get_samples(node.attribute2int("samples")),
		PathTools::join(
			full_path_of_original_xml_file.dirname, 
			node.attribute("file")
		)
	);
}
//------------------------------------------------------------------------------
bool FunctionFab::has_function(const string key)const {
	return functions.find(key) != functions.end();
}
//------------------------------------------------------------------------------
void FunctionFab::assert_name_not_in_use_yet(const string key)const {
	if(has_function(key)) {
		stringstream info;
		info << "FunctionFab " << __FILE__ << ", " << __LINE__ << "\n";
		info << "| redecleration of function " << key <<"\n";
		throw TracerException(info.str());
	}
}
//------------------------------------------------------------------------------
void FunctionFab::assert_is_function_node(const Xml::Node &node)const {
	if(!is_function_node(node)) {
		stringstream info;
		info << "FunctionFab " << __FILE__ << ", " << __LINE__ << "\n";
		info << "Expected node name to be: function, but actually: ";
		info << node.name() <<"\n";
		throw TracerException(info.str());
	}
}
//------------------------------------------------------------------------------
bool FunctionFab::is_function_node(const Xml::Node &node)const {
	return is_equal(node.name(),"function");
}
//------------------------------------------------------------------------------
Function::Func1D* FunctionFab::extract_linear_interpolation(const Xml::Node &node) {

	vector<vector<double>> table;
	for(Xml::Node child=node.first_child();	child; child=child.next_child()) {
		if(child.name(), "xy") {
			vector<double> row = {
				child.attribute2double("x"),
				child.attribute2double("y")
			};
			table.push_back(row);
		}
	}
	return new Function::LinInterpol(table);
}
//------------------------------------------------------------------------------
Function::Func1D* FunctionFab::extract_constant(const Xml::Node &node) {
	return new Function::Constant(
		node.attribute2double("value"),
		extract_limits_from_attributes(node)
	);
}
//------------------------------------------------------------------------------
Function::Func1D* FunctionFab::extract_polynom3(const Xml::Node &node) {
	return new Function::Polynom3(
		node.attribute2double("x3"),
		node.attribute2double("x2"),
		node.attribute2double("x1"),
		node.attribute2double("x0"),
		extract_limits_from_attributes(node)
	);
}
//------------------------------------------------------------------------------
Function::Func1D* FunctionFab::extract_concatenation(const Xml::Node &node) {
	string attribute = "f0";
	std::vector<const Function::Func1D*> funcs;
	uint count = 0;

	while (node.has_attribute(attribute)) {

		funcs.push_back(by_name(node.attribute(attribute)));
		count++;
		stringstream satt; satt << "f" << count;
		attribute = satt.str();
	};

	return new Function::Concat(funcs);
}
//------------------------------------------------------------------------------
Function::Limits FunctionFab::extract_limits_from_attributes(const Xml::Node &node) {
	return Function::Limits(
		node.attribute2double("lower_limit"), 
		node.attribute2double("upper_limit")
	);
}
//------------------------------------------------------------------------------
const Function::Func1D* FunctionFab::by_name(const string name)const {
	assert_has_function(name);
	return functions.find(name)->second;
}
//------------------------------------------------------------------------------
void FunctionFab::assert_has_function(const string name)const {
	if(!has_function(name)) {
		stringstream info;
		info << "FunctionFab " << __FILE__ << ", " << __LINE__ << "\n";
		info << "There is no function called '" << name << "'. \n";
		throw TracerException(info.str());
	}
}