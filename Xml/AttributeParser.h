//=================================
// include guard
#ifndef __Xml_AttributeParser_H_INCLUDED__
#define __Xml_AttributeParser_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Node.h"
#include "Problem.h"
#include "Tools/Tuple3.h"
#include "Core/Vector3D.h"
#include "Core/Rotation3D.h"
#include "Core/Color.h"

namespace Xml {

    class AttributeIsNoDouble : public TracerException {
        using TracerException::TracerException;
    }; 

    double attribute_to_double(Node node, std::string attribute_name) {

        std::string attribute = node.get_attribute(attribute_name);
        double number;
        
        try{
            
            number = StringTools::to_double(attribute);
        }catch(std::exception &error) {

            Problem problem(node);

            std::stringstream info;
            info << __FILE__ << ", " << __LINE__ << "\n\n";
            info << "In Xml file: '" << problem.get_path() << ", ";
            info << "line " << problem.get_line() << ", ";
            info << "column " << problem.get_column() << "\n";
            info << "In node '" << node.get_name() << "' ";
            info << "can not parse attribute '" << attribute_name << "' ";
            info << "to double.\n\n";
            info << problem.get_problem_section_from_original_file() << "\n\n";
            info << error.what() << "\n";
            throw AttributeIsNoDouble(info.str());
        }

        return number;
    }
//------------------------------------------------------------------------------
    class AttributeIsNoTuple3 : public TracerException {
        using TracerException::TracerException;
    }; 

    Tuple3 attribute_to_Tuple3(Node node, std::string attribute_name) {

        std::string attribute = node.get_attribute(attribute_name);
        Tuple3 t3;
        
        try{
            
            t3 = StringTools::to_Tuple3(attribute);
        }catch(std::exception &error) {

            Problem problem(node);

            std::stringstream info;
            info << __FILE__ << ", " << __LINE__ << "\n\n";
            info << "In Xml file: '" << problem.get_path() << ", ";
            info << "line " << problem.get_line() << ", ";
            info << "column " << problem.get_column() << "\n";
            info << "In node '" << node.get_name() << "' ";
            info << "can not parse attribute '" << attribute_name << "' ";
            info << "to Tuple3.\n\n";
            info << problem.get_problem_section_from_original_file() << "\n\n";
            info << error.what() << "\n";
            throw AttributeIsNoTuple3(info.str());
        }

        return t3;
    }
    //--------------------------------------------------------------------------
    class AttributeIsNoVector3D : public TracerException {
        using TracerException::TracerException;
    }; 

    Vector3D attribute_to_Vector3D(Node node, std::string attribute_name) {

        try{

            Tuple3 t3 = attribute_to_Tuple3(node, attribute_name);
            return Vector3D(t3.x, t3.y, t3.z);
        }catch(std::exception &error) {

            std::stringstream info;
            info << "Can not parse to Vector3D.\n";
            info << "\n\n" << error.what() << "\n";
            throw AttributeIsNoVector3D(info.str());
        }
    }
    //--------------------------------------------------------------------------
    class AttributeIsNoRotation3D : public TracerException {
        using TracerException::TracerException;
    }; 

    Rotation3D attribute_to_Rotation3D(Node node, std::string attribute_name) {

        try{

            Tuple3 t3 = attribute_to_Tuple3(node, attribute_name);
            return Rotation3D(t3.x, t3.y, t3.z);
        }catch(std::exception &error) {

            std::stringstream info;
            info << "Can not parse to Rotation3D.\n";
            info << "\n\n" << error.what() << "\n";
            throw AttributeIsNoRotation3D(info.str());
        }
    }
    //--------------------------------------------------------------------------
    class AttributeIsNoColor : public TracerException {
        using TracerException::TracerException;
    }; 

    Color attribute_to_Color(Node node, std::string attribute_name) {

        try{

            Tuple3 t3 = attribute_to_Tuple3(node, attribute_name);
            return Color(t3.x, t3.y, t3.z);
        }catch(std::exception &error) {

            std::stringstream info;
            info << "Can not parse to Color.\n";
            info << "\n\n" << error.what() << "\n";
            throw AttributeIsNoColor(info.str());
        }
    }
}//Xml
#endif // __Xml_AttributeParser_H_INCLUDED__
