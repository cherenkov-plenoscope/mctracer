//=================================
// include guard
#ifndef __XmlNode_H_INCLUDED__
#define __XmlNode_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "pugi/pugixml.hpp"
#include <string>
#include <memory>

//------------------------------------------------------------------------------
namespace Xml {

	class Node {
		friend class Problem;

		pugi::xml_node node;
		const std::shared_ptr<std::string> file;
		const std::string path;
	public:

		void operator=(const Node eq);
		Node(
			const pugi::xml_node _node, 
			const std::shared_ptr<std::string> _file,
			const std::string _path
		);
		std::string name()const;
		std::string attribute(const std::string attribute)const;
		Node child(const std::string child)const;
		Node first_child()const;
		Node next_child();
		operator bool()const { 
		    return node ? true: false; 
		}
	
		bool has_child(const std::string child)const;
		bool has_attribute(const std::string attribute)const;
		std::string xml_path()const;
	private:

		void assert_child_exists(const std::string child)const;
		void assert_attribute_exists(const std::string attribute)const;
	};	
}//Xml
#endif // __XmlNode_H_INCLUDED__ 
