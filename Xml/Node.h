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
using std::string;
//------------------------------------------------------------------------------
namespace Xml {

	class Node {
		friend class Problem;

		pugi::xml_node node;
		const std::shared_ptr<string> file;
		const string path;
	public:

		void operator=(const Node eq);
		Node(
			const pugi::xml_node _node, 
			const std::shared_ptr<string> _file,
			const string _path
		);
		string name()const;
		string attribute(const string attribute)const;
		Node child(const string child)const;
		Node first_child()const;
		Node next_child();
		operator bool()const { 
		    return node ? true: false; 
		}
	
		bool has_child(const string child)const;
		bool has_attribute(const string attribute)const;
		string xml_path()const;
	private:

		void assert_child_exists(const string child)const;
		void assert_attribute_exists(const string attribute)const;
	};	
}//Xml
#endif // __XmlNode_H_INCLUDED__ 
