#include "XmlIoException.h"
//------------------------------------------------------------------------------
XmlIoException::XmlIoException(std::string msg,const XmlIoFileInfo *file_info){
	set_up(msg,file_info);
}
//------------------------------------------------------------------------------
XmlIoException::XmlIoException(){}
//------------------------------------------------------------------------------
void XmlIoException::set_up(std::string msg,const XmlIoFileInfo *file_info){
	message 		= msg;
	Filename		= file_info->get_XmlName();
	parsing_result 	= file_info->get_XmlResult();

	if( build_offset_data_to_locate_line_and_column(
			file_info->get_XmlName(), 
			file_info->get_XmlNode().offset_debug()
		) 
	){
		file_can_be_opened_and_therefore_has_offset_info = true;
	}else{
		file_can_be_opened_and_therefore_has_offset_info = false;
	}	
}
//------------------------------------------------------------------------------
std::string XmlIoException::get_XmlIo_print()const{
	std::stringstream out;
	out << "| File: '" << Filename << "'";
	if( file_can_be_opened_and_therefore_has_offset_info ){
		out << ", line: " << line << ", column: " << column;
	}else{
		out << " can not be opened.";
	}
	out << "\n";
	out << "|\n";
	out << "| pugi::xml\n";
	out << "| Description: " << parsing_result.description() << "\n";

	//if( parsing_result.encoding != pugi::status_ok ){
	//out << "| Encoding: " << pugi_encoding_to_str(parsing_result.encoding);
	//out << "\n";
	//}
	out << "|\n";
	return out.str();	
}
//------------------------------------------------------------------------------
std::string XmlIoException::get_message_print()const{
	std::stringstream out;
	out << "| " << message << "\n";
	out << "|\n";
	return out.str();
}
//------------------------------------------------------------------------------
const char * XmlIoException::what () const throw (){
	std::stringstream msg;
	msg << " ____xml_IO_Exception____________________________\n|\n";
	msg << get_message_print();
	msg << get_XmlIo_print();
	return str2chararray(msg.str());
}
//------------------------------------------------------------------------------
// Pugi XML can not give us the line number and the column in a xml file 
// where parsing errors occured. However it gives us the number of the 
// character in the effected xml file where the error took place. To get now
// the line and column information we use the following snippset found on
// StackOverflow: Convert pugixml's result.offset to column/line
//
// Author: zeuxcg
// This functionality is not readily available in pugixml since it's 
// relatively expensive to do it on every parse, and after parsing is
// complete it's impossible to recover file/line information in the 
// general case.
//
// Here's a snippet that builds an offset -> line mapping that you can
// use in case parsing fails or you need the information for other reasons; 
// feel free to tweak file I/O code to match your requirements.
bool XmlIoException::build_offset_data_to_locate_line_and_column(
	const std::string file, const ptrdiff_t offset_found_by_pugi_xml
){

	std::vector<ptrdiff_t> result;

    FILE* f = fopen(file.c_str(), "rb");
    if (!f){
    	return false;
    }

    ptrdiff_t offset = 0;

    char buffer[1024];
    size_t size;

    while ((size = fread(buffer, 1, sizeof(buffer), f)) > 0)
    {
    for (size_t i = 0; i < size; ++i)
        if (buffer[i] == '\n')
            result.push_back(offset + i);

    offset += size;
    }

    fclose(f);

	get_line_and_column_location(result, offset_found_by_pugi_xml);

    return true;
}
//------------------------------------------------------------------------------
void XmlIoException::get_line_and_column_location(
	const std::vector<ptrdiff_t>& data,
	ptrdiff_t offset
){

    std::vector<ptrdiff_t>::const_iterator it = 
    std::lower_bound(data.begin(), data.end(), offset);

    size_t index = it - data.begin();

    line 	= 1 + index;
    column 	= index == 0 ? offset + 1 : offset - data[index - 1];
}
//------------------------------------------------------------------------------
std::string XmlIoException::pugi_encoding_to_str(int encoding)const{
	// The pugi xml error encodings with detailed informaion
	// http://pugixml.googlecode.com/svn/tags/latest/docs/manual/loading.html
	switch(encoding){
		case pugi::status_ok:
			return"status_ok means that no error was encountered during parsing; the source stream represents the valid XML document which was fully parsed and converted to a tree.";
		break;
		case pugi::status_file_not_found:
			return"status_file_not_found is only returned by load_file function and means that file could not be opened.";
		break;
		case pugi::status_io_error:
			return"status_io_error is returned by load_file function and by load functions with std::istream/std::wstream arguments; it means that some I/O error has occurred during reading the file/stream.";
		break;
		case pugi::status_out_of_memory:
			return"status_out_of_memory means that there was not enough memory during some allocation; any allocation failure during parsing results in this error.";
		break;
		case pugi::status_internal_error:
			return"status_internal_error means that something went horribly wrong; currently this error does not occur.";
		break;
		case pugi::status_unrecognized_tag:
			return"status_unrecognized_tag means that parsing stopped due to a tag with either an empty name or a name which starts with incorrect character, such as #.";
		break;
		case pugi::status_bad_pi:
			return"status_bad_pi means that parsing stopped due to incorrect document declaration/processing instruction.";
		break;
		case pugi::status_bad_comment:
			return"status_bad_comment, status_bad_cdata, status_bad_doctype and status_bad_pcdata mean that parsing stopped due to the invalid construct of the respective type.";
		break;
		case pugi::status_bad_start_element:
			return"status_bad_start_element means that parsing stopped because starting tag either had no closing > symbol or contained some incorrect symbol.";
		break;
		case pugi::status_bad_attribute:
			return"status_bad_attribute means that parsing stopped because there was an incorrect attribute, such as an attribute without value or with value that is not quoted (note that <node attr=1> is incorrect in XML)";
		break;
		case pugi::status_bad_end_element:
			return"status_bad_end_element means that parsing stopped because ending tag had incorrect syntax (i.e. extra non-whitespace symbols between tag name and >).";
		break;
		case pugi::status_end_element_mismatch:
			return"status_end_element_mismatch means that parsing stopped because the closing tag did not match the opening one (i.e. <node></nedo>) or because some tag was not closed at all.";
		break;
		// this seems not to be supported by the pugi xml used here
		//case pugi::status_no_document_element:
		//	return"status_no_document_element means that no element nodes were discovered during parsing; this usually indicates an empty or invalid document.";
		break;
		default:
			return"Unknown pugi::xml encoding";
	};
}
//------------------------------------------------------------------------------
UnknownItem::UnknownItem(
	std::string msg,
	const XmlIoFileInfo *file_info, 
	std::string name_of_unknown_item
){
	set_up(msg,file_info);
	this->name_of_unknown_item = name_of_unknown_item;
}
//------------------------------------------------------------------------------
MissingItem::MissingItem(
	std::string msg,
	const XmlIoFileInfo *file_info, 
	std::string name_of_missing_item
){
	set_up(msg,file_info);
	this->name_of_missing_item = name_of_missing_item;
}
//------------------------------------------------------------------------------
MultipleUseage::MultipleUseage(
	std::string msg,
	const XmlIoFileInfo *file_info, 
	std::string path_of_frame_multiple_in_use
){
	set_up(msg,file_info);
	this->path_of_frame_multiple_in_use = path_of_frame_multiple_in_use;
}
//------------------------------------------------------------------------------
BadAttribute::BadAttribute(
	std::string msg,
	const XmlIoFileInfo *file_info, 
	std::string attribute_value
){
	set_up(msg,file_info);
	this->attribute_value = attribute_value;
}