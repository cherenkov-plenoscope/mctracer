#include "Node.h"
#include "Exceptions.h"
#include "Problem.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include "Exceptions.h"
//------------------------------------------------------------------------------
namespace Xml  {
//------------------------------------------------------------------------------
void Node::operator=(const Node eq) {
	node = eq.node;
}

Node::Node(
	const pugi::xml_node _node, 
	const std::shared_ptr<std::string> _file,
	const std::string _path
):node(_node), file(_file), path(_path){

}

std::string Node::get_name()const {
	return node.name();
}

std::string Node::get_attribute(const std::string att)const {
	assert_attribute_exists(att);
	std::cout << att << ", >" << node.attribute(att.c_str()).value() << "<\n";
	return node.attribute(att.c_str()).value();
}

Node Node::get_child(const std::string child)const {
	assert_child_exists(child);
	return Node(node.child(child.c_str()), file, path);
}

Node Node::get_first_child()const {
	return Node(node.first_child(), file, path);
}

Node Node::get_next_child() {
	return Node(node.next_sibling(), file, path);
}

bool Node::has_child(const std::string child)const {
	return node.child(child.c_str()) != nullptr;
}

bool Node::has_attribute(const std::string attribute)const {
	return node.attribute(attribute.c_str()) != nullptr;
}

void Node::assert_child_exists(const std::string child)const {

	if(!has_child(child)) {

		Problem problem(file, node.offset_debug(), path);

		std::stringstream info;
		info << __FILE__ << ", " << __LINE__ << "\n\n";
		info << "In xml file: '" << problem.get_path() << "', ";
		info << "line " << problem.get_line() << ", " << problem.get_column() << "\n";
		info << "The Xml Node '" << get_name() << "' has no child called ";
		info << "'" << child << "'.\n\n";
		info << problem.get_problem_section_from_original_file();
		throw NoSuchChild(info.str());
	}
}	

void Node::assert_attribute_exists(const std::string attribute)const {

	if(has_attribute(attribute)){

		Problem problem(file, node.offset_debug(), path);

		std::stringstream info;
		info << __FILE__ << ", " << __LINE__ << "\n\n";
		info << "In xml file: '" << problem.get_path() << "', ";
		info << "line " << problem.get_line() << ", " << problem.get_column() << "\n";
		info << "The Xml Node '" << get_name() << "' has no attribute called ";
		info << "'" << attribute << "'.\n\n";
		info << problem.get_problem_section_from_original_file();
		throw NoSuchAttribute(info.str());
	}	
}
//------------------------------------------------------------------------------
}//Xml