//=================================
// include guard
#ifndef __XMLIOEXCEPTION_H_INCLUDE__
#define __XMLIOEXCEPTION_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "TracerException.h"
#include "Xml/pugi/pugixml.hpp"
#include "XmlFileIo.h"
//------------------------------------------------------------------------------
class XmlIoException :public TracerException{

	bool file_can_be_opened_and_therefore_has_offset_info = false;

	uint line = 0;
	uint column = 0;

	std::string Filename;
	pugi::xml_parse_result parsing_result;
public:
	XmlIoException();

	void set_up(std::string msg,const XmlIoFileInfo *file_info);

	XmlIoException(std::string msg,const XmlIoFileInfo *file_info);

	std::string filename()const{
		return Filename;
	};

	std::string get_XmlIo_print()const;
	std::string get_message_print()const;

	virtual const char * what () const throw ();
private:
	std::string pugi_encoding_to_str(int encoding)const;
	bool build_offset_data_to_locate_line_and_column(
		const std::string file, const ptrdiff_t offset_found_by_pugi_xml
	);

	void get_line_and_column_location(
		const std::vector<ptrdiff_t>& data,
		ptrdiff_t offset
	);
};
//------------------------------------------------------------------------------
class UnknownItem : public XmlIoException{
public:
	std::string name_of_unknown_item;

	UnknownItem(
		std::string msg,
		const XmlIoFileInfo *file_info, 
		std::string name_of_unknown_item
	);

	std::string name()const{
		return name_of_unknown_item;
	}

	std::string get_UnknownItem_print()const{
		std::stringstream out;
		out << "| unknown item: '" << name_of_unknown_item << "'\n";
		out << "|\n";
		return out.str();
	}

	const char * what () const throw (){	
		std::stringstream msg;
		msg << " ____UnknownItem_Exception_______________________\n|\n";
		msg << get_UnknownItem_print();
		msg << get_message_print();
		msg << get_XmlIo_print();
		return str2chararray(msg.str());
	}
};
//------------------------------------------------------------------------------
class MissingItem : public XmlIoException{
public:
	std::string name_of_missing_item;

	MissingItem(
		std::string msg,
		const XmlIoFileInfo *file_info,
		std::string name_of_missing_item);

	std::string name()const{
		return name_of_missing_item;
	}

	std::string get_MissingItem_print()const{
		std::stringstream out;
		out << "| missing item: '" << name_of_missing_item << "'\n";
		out << "|\n";
		return out.str();
	}

	const char * what () const throw (){
		std::stringstream msg;
		msg << " ____MissingItem_Exception_______________________\n|\n";
		msg << get_MissingItem_print();
		msg << get_message_print();
		msg << get_XmlIo_print();
		return str2chararray(msg.str());
	}
};
//------------------------------------------------------------------------------
class MultipleUseage : public XmlIoException{
public:
	std::string path_of_frame_multiple_in_use;

	MultipleUseage(
		std::string msg,
		const XmlIoFileInfo *file_info,
		std::string path_of_frame_multiple_in_use);

	std::string name()const{
		return path_of_frame_multiple_in_use;
	}

	std::string get_MultipleUseage_print()const{
		std::stringstream out;
		out << "| multiple usage: '" << path_of_frame_multiple_in_use << "'\n";
		out << "|\n";
		return out.str();
	}

	const char * what () const throw (){
		std::stringstream msg;
		msg << " ____MultipleUseage_Exception________________\n|\n";
		msg << get_MultipleUseage_print();
		msg << get_message_print();
		msg << get_XmlIo_print();
		return str2chararray(msg.str());
	}
};
//------------------------------------------------------------------------------
class BadAttribute : public XmlIoException{
public:
	std::string attribute_value;

	BadAttribute(
		std::string msg,
		const XmlIoFileInfo *file_info,
		std::string attribute_value);

	std::string name()const{
		return attribute_value;
	}

	std::string get_BadAttribute_print()const{
		std::stringstream out;
		out << "| attribute value: '" << attribute_value << "'\n";
		out << "|\n";
		return out.str();
	}

	const char * what () const throw (){
		std::stringstream msg;
		msg << " ____BadAttribute_Exception__________________\n|\n";
		msg << get_BadAttribute_print();
		msg << get_message_print();
		msg << get_XmlIo_print();
		return str2chararray(msg.str());		
	}
};
//------------------------------------------------------------------------------

#endif //__XMLIOEXCEPTION_H_INCLUDE__
