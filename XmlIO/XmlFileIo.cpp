#include "XmlFileIo.h"
//------------------------------------------------------------------------------
double XmlFileIo::StrToDouble(std::string text_to_parse)const{

	if(text_to_parse.compare("") == 0){
		std::stringstream info;
		info << "WorldFactory::"<<__func__<<"() ";
		info << "attribute string is empty.";
		throw BadAttribute(info.str(), this, text_to_parse);
	}
	
	char * e;
	double number_parsed_in = std::strtod(text_to_parse.c_str(), &e);

	if (*e != 0){
		std::stringstream info;
		info << "WorldFactory::"<<__func__<<"() ";
		info << "can not convert attribute string to floating point number.";
		throw BadAttribute(info.str(), this, text_to_parse);
	}
	return number_parsed_in;
}
//------------------------------------------------------------------------------
void XmlFileIo::strto3tuple(
	double &x, double &y, double &z,const std::string text)const{
	// A text string of the form "[f,f,f]" is parsed in three floating point
	// numbers x,y and z
	std::string TextToWorkOn = text;
	
	std::size_t pos = TextToWorkOn.find("[");
	if(pos != std::string::npos){
		TextToWorkOn = TextToWorkOn.substr(pos+1);	
	}else{
		std::stringstream info;
		info << "Error parsing tuple: [float,float,float] ";
		info << "I can not find the opening '[' in '" << text << "' !";
		throw BadAttribute(info.str(), this, text);
	}
	
	pos = TextToWorkOn.find(",");
	if(pos != std::string::npos){
		x = StrToDouble(TextToWorkOn.substr(0,pos));	
		TextToWorkOn = TextToWorkOn.substr(pos+1);
	}else{
		std::stringstream info;
		info << "Error parsing tuple: [float,float,float] "; 
		info << "I can not find the comma ',' in '" << text << "' !";
		throw BadAttribute(info.str(), this, text);
	}
	
	pos = TextToWorkOn.find(",");
	if(pos != std::string::npos){
		y = StrToDouble(TextToWorkOn.substr(0,pos));
		TextToWorkOn = TextToWorkOn.substr(pos+1);
	}else{
		std::stringstream info;
		info << "Error parsing tuple: [float,float,float] ";  
		info << "I can not find the comma ',' in '" << text << "' !";
		throw BadAttribute(info.str(), this, text);
	}	

	pos = TextToWorkOn.find("]");
	if(pos != std::string::npos){
		z = StrToDouble(TextToWorkOn.substr(0,pos));
	}else{
		std::stringstream info;
		info << "Error parsing tuple: [float,float,float]";  
		info << "I can not find the closing ']' in '" << text << "' ! ";
		throw BadAttribute(info.str(), this, text);
	}	
}
//------------------------------------------------------------------------------
void XmlFileIo::assert_child_exists(
	const pugi::xml_node &node, const std::string child_name){

	XmlNode = node;

	if( node.child(child_name.c_str()) == nullptr ){
		std::stringstream info;
		info << "Assert existence of the node called: '" << child_name;
		info << "'...";
		throw MissingItem( info.str(), this, child_name);
	}
}	
//------------------------------------------------------------------------------
void XmlFileIo::assert_attribute_exists(
		const pugi::xml_node &node, const std::string attribute_name){

	XmlNode = node;

	if( node.attribute(attribute_name.c_str()) == nullptr ){
		std::stringstream info;
		info << "Assert existence of the attribute called: '" << attribute_name;
		info << "'...";
		throw MissingItem( info.str(), this, attribute_name);
	}	
}
//------------------------------------------------------------------------------
void XmlFileIo::assert_node_name(
		const pugi::xml_node &node, const std::string node_name){

	XmlNode = node;

	if( node_name.compare(node.name()) != 0 ){
		std::stringstream info;
		info << "Assert name of node: '" << node_name << "'. ";
		info << "Actual: '" << node.name() << "' ";
		info << "Expected: '" << node_name << "'.";		
		throw XmlIoException( info.str(), this);
	}	
}