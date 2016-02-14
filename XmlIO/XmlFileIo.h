//=================================
// include guard
#ifndef __XMLFILEIO_H_INCLUDED__
#define __XMLFILEIO_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Xml/pugi/pugixml.hpp"
#include "TracerException.h"
#include "Tools/Tools.h"
//------------------------------------------------------------------------------
class XmlIoFileInfo{
public:

	std::string get_XmlName()const{return XmlName;};
	std::string get_XmlRelativePath()const{return XmlRelativePath;};
	pugi::xml_node get_XmlNode()const{return XmlNode;};
	pugi::xml_parse_result get_XmlResult()const{return XmlResult;};
protected:

	std::string 			XmlName = "";
	std::string XmlRelativePath;
	pugi::xml_node 			XmlNode;
	pugi::xml_parse_result 	XmlResult;
};
//------------------------------------------------------------------------------
#include "XmlIoException.h"

class XmlFileIo: public XmlIoFileInfo{
public:
	double StrToDouble(std::string text_to_parse)const;

	void strto3tuple(
		double &x, 
		double &y, 
		double &z,
		const std::string text
	)const;

	bool has_child(
		const pugi::xml_node &node, const std::string child_name);

	void assert_child_exists(
		const pugi::xml_node &node, const std::string child_name);

	void assert_attribute_exists(
		const pugi::xml_node &node, const std::string attribute_name);

	bool has_attribute(
		const pugi::xml_node &node, const std::string attribute_name);

	void assert_node_name(
		const pugi::xml_node &node, const std::string node_name);
};
#endif // __XMLFILEIO_H_INCLUDED__ 
