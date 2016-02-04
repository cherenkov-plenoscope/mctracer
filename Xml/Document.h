//=================================
// include guard
#ifndef __XmlDocument_H_INCLUDED__
#define __XmlDocument_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "XmlIO/pugixml.hpp"
#include "Node.h"
#include <vector>
#include <string>
#include <memory>

//------------------------------------------------------------------------------
namespace Xml {

	class Document {

		std::string path;
		std::shared_ptr<std::string> file;
		pugi::xml_document pugi_doc;
		pugi::xml_parse_result pugi_result;
	public:

		Document(const std::string path);
		Node get_tree()const;
		std::string get_info_print(Node node);
	private:

		void read_in_documnet();
		void assert_xml_file_is_valid();
		std::string pugi_status_msg(int encoding)const;
	};
}
#endif // __XmlDocument_H_INCLUDED__ 
