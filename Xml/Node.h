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
#include "Tools/Tuple3.h"
#include "Core/Vec3.h"
#include "Core/Rot3.h"
#include "Core/Color.h"
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

		int attribute2int(const string attribute_name)const;
		double attribute2double(const string attribute_name)const;
		bool attribute2bool(const string attribute_name)const;
		Tuple3 attribute2Tuple3(const string attribute_name)const;
		Vec3 attribute2Vec3(const string attribute_name)const;
		Rot3 attribute2Rot3(const string attribute_name)const;
		Color attribute2Color(const string attribute_name)const;

	    class AttributeIsNoInt : public std::invalid_argument {
	        using invalid_argument::invalid_argument;
	    }; 

	    class AttributeIsNoDouble : public std::invalid_argument {
	        using invalid_argument::invalid_argument;
	    }; 

	    class AttributeIsNoBool : public std::invalid_argument {
	        using invalid_argument::invalid_argument;
	    }; 

	    class AttributeIsNoTuple3 : public std::invalid_argument {
	        using invalid_argument::invalid_argument;
	    }; 

	    class AttributeIsNoVec3 : public std::invalid_argument {
	        using invalid_argument::invalid_argument;
	    }; 
	    
	    class AttributeIsNoRot3 : public std::invalid_argument {
	        using invalid_argument::invalid_argument;
	    }; 
	    
	    class AttributeIsNoColor : public std::invalid_argument {
	        using invalid_argument::invalid_argument;
	    }; 
	private:

		void assert_child_exists(const string child)const;
		void assert_attribute_exists(const string attribute)const;
	};	
}//Xml
#endif // __XmlNode_H_INCLUDED__ 
