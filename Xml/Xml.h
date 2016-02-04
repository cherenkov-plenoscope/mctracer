//=================================
// include guard
#ifndef __Xml_H_INCLUDED__
#define __Xml_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Document.h"
#include "Node.h"
#include "Exceptions.h"
#include "Problem.h"

namespace Xml {

	class AttributeIsNoDouble : public TracerException {
		using TracerException::TracerException;
	}; 

	double attribute_as_double(Node node, std::string attribute_name) {

		std::string attribute = node.get_attribute(attribute_name);
		std::cout << "attr: " << attribute << "\n";
		double number;
		
		try{
			
			number = StrToDouble(attribute);
		}catch(std::exception &error) {

			Problem problem(node);

			std::stringstream info;
        	info << __FILE__ << ", " << __LINE__ << "\n\n";
       		info << "In Xml file: '" << problem.get_path() << ", ";
       		info << "line " << problem.get_line() << ", column " << problem.get_column() << "\n";
       		info << "In node '" << node.get_name() << "' ";
       		info << "can not parse attribute '" << attribute_name << "' ";
       		info << "as double.\n\n";
       		info << problem.get_problem_section_from_original_file() << "\n\n";
       		info << error.what() << "\n";
			throw AttributeIsNoDouble(info.str());
		} 

		return number;
	}
}//Xml

#endif // __Xml_H_INCLUDED__ 
