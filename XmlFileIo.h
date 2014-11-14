//=================================
// include guard
#ifndef __XMLFILEIO_H_INCLUDED__
#define __XMLFILEIO_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "pugixml.hpp"
#include "TracerException.h"
//=================================
class XmlFileIo{
public:

	XmlFileIo();
	std::string 			get_XmlName()const{return XmlName;};
	pugi::xml_node 			get_XmlNode()const{return XmlNode;};
	pugi::xml_parse_result 	get_XmlResult()const{return XmlResult;};
protected:

	std::string 			XmlName = "";
	pugi::xml_node 			XmlNode;
	pugi::xml_parse_result 	XmlResult;
};
#endif // __XMLFILEIO_H_INCLUDED__ 
