#include "FunctionFactory.h"
#include "Tools/StringTools.h"
#include "Tools/AsciiIo.h"
#include "Core/Function/LinInterpolFunction.h"
#include "Core/Function/ConstantFunction.h"
#include "Core/Function/Polynom3Function.h"
#include "Core/Function/ConcatFunction.h"

//------------------------------------------------------------------------------
FunctionFactory::FunctionFactory(const XmlFileIo* io_to_take_over) {
	mother_factory = io_to_take_over;
	/*XmlName = mother_factory->get_XmlName();
	XmlNode = mother_factory->get_XmlNode();
	XmlResult = mother_factory->get_XmlResult();
	XmlRelativePath = mother_factory->get_XmlRelativePath();*/
}
//------------------------------------------------------------------------------
void FunctionFactory::extract_function_from(const pugi::xml_node node) {
	XmlNode = node;
	std::string name = extract_name(node);
	Function::Func1D* func;

	for(
		pugi::xml_node sub_node = node.first_child(); 
		sub_node; 
		sub_node = sub_node.next_sibling()
	){	
		std::string sub_node_name = sub_node.name();

		if 		(StringTools::is_equal(sub_node_name,"linear_interpolation")){
			func = extract_linear_interpolation(sub_node);
		}else if(StringTools::is_equal(sub_node_name,"constant")){
			func = extract_constant(sub_node);
		}else if(StringTools::is_equal(sub_node_name,"polynom3")){
			func = extract_polynom3(sub_node);
		}else if(StringTools::is_equal(sub_node_name,"concatenation")){
			func = extract_concatenation(sub_node);
		}else{
			std::stringstream info;
			info << "FunctionFactory " << __FILE__ << ", " << __LINE__;
			throw UnknownItem(info.str(), mother_factory, sub_node_name);
		}
	}

	functions.insert(std::pair<std::string, Function::Func1D*>(name, func));
}
//------------------------------------------------------------------------------
std::string FunctionFactory::extract_name(const pugi::xml_node node) {
	assert_attribute_exists(node, "name");
	assert_is_function_node(node);
	std::string name = node.attribute("name").value();
	assert_name_is_unique(name);
	return name;
}
//------------------------------------------------------------------------------
bool FunctionFactory::has_function(const std::string key)const {
	return functions.find(key) != functions.end();
}
//------------------------------------------------------------------------------
void FunctionFactory::assert_name_is_unique(const std::string key)const {
	if(has_function(key)) {
		std::stringstream info;
		info << "FunctionFactory " << __FILE__ << ", " << __LINE__ << "\n";
		info << "| redecleration of function " << key <<"\n";
		throw MultipleUseage(info.str(), mother_factory, key);
	}
}
//------------------------------------------------------------------------------
void FunctionFactory::assert_is_function_node(const pugi::xml_node node)const {
	if(!is_function_node(node)) {
		std::stringstream info;
		info << "FunctionFactory " << __FILE__ << ", " << __LINE__ << "\n";
		info << "| Expected node name to be: function, but actually: ";
		info << node.name() <<"\n";
		throw TracerException(info.str());
	}
}
//------------------------------------------------------------------------------
bool FunctionFactory::is_function_node(const pugi::xml_node node)const {
	return StringTools::is_equal(node.name(),"function");
}
//------------------------------------------------------------------------------
Function::Func1D* FunctionFactory::extract_linear_interpolation(
	const pugi::xml_node node
) {
	assert_attribute_exists(node, "file");
	std::vector<std::vector<double>> table = AsciiIo::gen_table_from_file(
		mother_factory->get_XmlRelativePath() + node.attribute("file").value()
	);

	return new Function::LinInterpol(table);
}
//------------------------------------------------------------------------------
Function::Func1D* FunctionFactory::extract_constant(
	const pugi::xml_node node
) {
	assert_attribute_exists(node, "value");
	double value = StrToDouble(node.attribute("value").value());

	Function::Limits l = extract_limits_from_attributes(node); 

	return new Function::Constant(value, l);
}
//------------------------------------------------------------------------------
Function::Func1D* FunctionFactory::extract_polynom3(
	const pugi::xml_node node
) {
	assert_attribute_exists(node, "x3");
	double x3 = StrToDouble(node.attribute("x3").value());
	
	assert_attribute_exists(node, "x2");
	double x2 = StrToDouble(node.attribute("x2").value());

	assert_attribute_exists(node, "x1");
	double x1 = StrToDouble(node.attribute("x1").value());
	
	assert_attribute_exists(node, "x0");
	double x0 = StrToDouble(node.attribute("x0").value());

	Function::Limits l = extract_limits_from_attributes(node); 

	return new Function::Polynom3(x3, x2, x1, x0, l);
}
//------------------------------------------------------------------------------
Function::Func1D* FunctionFactory::extract_concatenation(
	const pugi::xml_node node
) {
	std::string attribute = "f0";
	std::vector<Function::Func1D*> funcs;
	uint count = 0;

	while (has_attribute(node, attribute)) {

		std::string func_to_be_added = node.attribute(attribute.c_str()).value();
		funcs.push_back(get_function(func_to_be_added));

		count++;
		std::stringstream satt; satt << "f" << count;
		attribute = satt.str();
	};

	return new Function::Concat(funcs);
}
//------------------------------------------------------------------------------
Function::Limits FunctionFactory::extract_limits_from_attributes(
	const pugi::xml_node node
) {
	
	assert_attribute_exists(node, "lower_limit");
	double lower_limit = StrToDouble(node.attribute("lower_limit").value());

	assert_attribute_exists(node, "upper_limit");
	double upper_limit = StrToDouble(node.attribute("upper_limit").value());

	return Function::Limits(lower_limit, upper_limit);
}
//------------------------------------------------------------------------------
Function::Func1D* FunctionFactory::get_function(const std::string name)const {
	assert_has_function(name);
	return functions.find(name)->second;
}
//------------------------------------------------------------------------------
void FunctionFactory::assert_has_function(const std::string name)const {
	if(!has_function(name)) {
		std::stringstream info;
		info << "FunctionFactory " << __FILE__ << ", " << __LINE__ << "\n";
		info << "| There is no function called '" << name << "'. \n";
		throw MissingItem(info.str(), mother_factory, name);
	}
}