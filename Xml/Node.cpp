#include "Node.h"
#include "Exceptions.h"
#include "Problem.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include "Exceptions.h"
#include "Tools/StringTools.h"
using std::stringstream;
//------------------------------------------------------------------------------
namespace Xml  {
//------------------------------------------------------------------------------
void Node::operator=(const Node eq) {
	node = eq.node;
}

Node::Node(
	const pugi::xml_node _node, 
	const std::shared_ptr<string> _file,
	const string _path
):node(_node), file(_file), path(_path){

}

string Node::name()const {
	return node.name();
}

string Node::attribute(const string att)const {
	assert_attribute_exists(att);
	return node.attribute(att.c_str()).value();
}

Node Node::child(const string child)const {
	assert_child_exists(child);
	return Node(node.child(child.c_str()), file, path);
}

Node Node::first_child()const {
	return Node(node.first_child(), file, path);
}

Node Node::next_child() {
	return Node(node.next_sibling(), file, path);
}

bool Node::has_child(const string child)const {
	return node.child(child.c_str()) != nullptr;
}

bool Node::has_attribute(const string attribute)const {
	return node.attribute(attribute.c_str()) != nullptr;
}

void Node::assert_child_exists(const string child)const {

	if(!has_child(child)) {

		Problem problem(file, node.offset_debug(), path);

		stringstream info;
		info << __FILE__ << ", " << __LINE__ << "\n\n";
		info << "In xml file: '" << problem.get_path() << "', ";
		info << "line " << problem.get_line() << ", ";
		info << problem.get_column() << "\n";
		info << "The Xml Node '" << name() << "' has no child called ";
		info << "'" << child << "'.\n\n";
		info << problem.get_problem_section_from_original_file();
		throw NoSuchChild(info.str());
	}
}	

void Node::assert_attribute_exists(const string attribute_name)const {

	if(!has_attribute(attribute_name)){

		Problem problem(file, node.offset_debug(), path);

		stringstream info;
		info << __FILE__ << ", " << __LINE__ << "\n\n";
		info << "In xml file: '" << problem.get_path() << "', ";
		info << "line " << problem.get_line() << ", ";
		info << problem.get_column() << "\n";
		info << "The Xml Node '" << name() << "' has no attribute called ";
		info << "'" << attribute_name << "'.\n\n";
		info << problem.get_problem_section_from_original_file();
		throw NoSuchAttribute(info.str());
	}	
}

string Node::xml_path()const {
	return path;
}

int Node::attribute2int(const string attribute_name)const {

    string att = attribute(attribute_name);
    int number;
    
    try{
        
        number = StringTools::to_int(att);
    }catch(std::exception &error) {

        Problem problem(file, node.offset_debug(), path);

        stringstream info;
        info << __FILE__ << ", " << __LINE__ << "\n\n";
        info << "In Xml file: '" << problem.get_path() << ", ";
        info << "line " << problem.get_line() << ", ";
        info << "column " << problem.get_column() << "\n";
        info << "In node '" << node.name() << "' ";
        info << "can not parse attribute '" << attribute_name << "' ";
        info << "to int.\n\n";
        info << problem.get_problem_section_from_original_file() << "\n\n";
        info << error.what() << "\n";
        throw AttributeIsNoInt(info.str());
    }

    return number;
}

double Node::attribute2double(const string attribute_name)const {

    string att = attribute(attribute_name);
    double number;
    
    try{
        
        number = StringTools::to_double(att);
    }catch(std::exception &error) {

        Problem problem(file, node.offset_debug(), path);

        stringstream info;
        info << __FILE__ << ", " << __LINE__ << "\n\n";
        info << "In Xml file: '" << problem.get_path() << ", ";
        info << "line " << problem.get_line() << ", ";
        info << "column " << problem.get_column() << "\n";
        info << "In node '" << node.name() << "' ";
        info << "can not parse attribute '" << attribute_name << "' ";
        info << "to double.\n\n";
        info << problem.get_problem_section_from_original_file() << "\n\n";
        info << error.what() << "\n";
        throw AttributeIsNoDouble(info.str());
    }

    return number;
}

bool Node::attribute2bool(const string attribute_name)const {

    string att = attribute(attribute_name);
    bool boolean;
    
    try{
        
        boolean = StringTools::to_bool(att);
    }catch(std::exception &error) {

        Problem problem(file, node.offset_debug(), path);

        stringstream info;
        info << __FILE__ << ", " << __LINE__ << "\n\n";
        info << "In Xml file: '" << problem.get_path() << ", ";
        info << "line " << problem.get_line() << ", ";
        info << "column " << problem.get_column() << "\n";
        info << "In node '" << node.name() << "' ";
        info << "can not parse attribute '" << attribute_name << "' ";
        info << "to bool.\n\n";
        info << problem.get_problem_section_from_original_file() << "\n\n";
        info << error.what() << "\n";
        throw AttributeIsNoBool(info.str());
    }

    return boolean;
}

Tuple3 Node::attribute2Tuple3(const string attribute_name)const {

    string att = attribute(attribute_name);
    Tuple3 t3;
    
    try{
        
        t3 = StringTools::to_Tuple3(att);
    }catch(std::exception &error) {

        Problem problem(file, node.offset_debug(), path);

        stringstream info;
        info << __FILE__ << ", " << __LINE__ << "\n\n";
        info << "In Xml file: '" << problem.get_path() << ", ";
        info << "line " << problem.get_line() << ", ";
        info << "column " << problem.get_column() << "\n";
        info << "In node '" << node.name() << "' ";
        info << "can not parse attribute '" << attribute_name << "' ";
        info << "to Tuple3.\n\n";
        info << problem.get_problem_section_from_original_file() << "\n\n";
        info << error.what() << "\n";
        throw AttributeIsNoTuple3(info.str());
    }

    return t3;
}

Vec3 Node::attribute2Vec3(const string attribute_name)const {

    try{

        Tuple3 t3 = attribute2Tuple3(attribute_name);
        return Vec3(t3.x, t3.y, t3.z);
    }catch(std::exception &error) {

        stringstream info;
        info << "Can not parse to Vec3.\n";
        info << "\n\n" << error.what() << "\n";
        throw AttributeIsNoVec3(info.str());
    }
}

Rot3 Node::attribute2Rot3(const string attribute_name)const {

    try{

        Tuple3 t3 = attribute2Tuple3(attribute_name);
        return Rot3(t3.x, t3.y, t3.z);
    }catch(std::exception &error) {

        stringstream info;
        info << "Can not parse to Rot3.\n";
        info << "\n\n" << error.what() << "\n";
        throw AttributeIsNoRot3(info.str());
    }
}

Color Node::attribute2Color(const string attribute_name)const {

    try{

        Tuple3 t3 = attribute2Tuple3(attribute_name);
        return Color(t3.x, t3.y, t3.z);
    }catch(std::exception &error) {

        stringstream info;
        info << "Can not parse to Color.\n";
        info << "\n\n" << error.what() << "\n";
        throw AttributeIsNoColor(info.str());
    }
}

//------------------------------------------------------------------------------
}//Xml