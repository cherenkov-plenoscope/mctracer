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
#include "Core/Rotation3D.h"
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
		Rotation3D attribute2Rotation3D(const string attribute_name)const;
		Color attribute2Color(const string attribute_name)const;

	    class AttributeIsNoInt : public TracerException {
	        using TracerException::TracerException;
	    }; 

	    class AttributeIsNoDouble : public TracerException {
	        using TracerException::TracerException;
	    }; 

	    class AttributeIsNoBool : public TracerException {
	        using TracerException::TracerException;
	    }; 

	    class AttributeIsNoTuple3 : public TracerException {
	        using TracerException::TracerException;
	    }; 

	    class AttributeIsNoVec3 : public TracerException {
	        using TracerException::TracerException;
	    }; 
	    
	    class AttributeIsNoRotation3D : public TracerException {
	        using TracerException::TracerException;
	    }; 
	    
	    class AttributeIsNoColor : public TracerException {
	        using TracerException::TracerException;
	    }; 
	private:

		void assert_child_exists(const string child)const;
		void assert_attribute_exists(const string attribute)const;
	};	
}//Xml
#endif // __XmlNode_H_INCLUDED__ 
