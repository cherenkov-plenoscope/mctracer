#include "BasicAttributes.h"

namespace Xml {
//------------------------------------------------------------------------------
std::string av(const std::string &att, const std::string &val) {
    std::stringstream out;
    out << att << "=\"" << val << "\"";
    return out.str();
}
//------------------------------------------------------------------------------
std::string comment(const std::string &text) {
    std::stringstream out;
    out << "<!-- " << text << " -->";
    return out.str();
}
//------------------------------------------------------------------------------
std::string to_string(const Vector3D &vec) {
    std::stringstream out;
    out << "[" << vec.x() << "," << vec.y() << "," <<  vec.z() << "]";
    return out.str();
}
//------------------------------------------------------------------------------
std::string to_string(const bool b) {
    return (b? "true" : "false");
}
//------------------------------------------------------------------------------
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
double attribute_to_bool(Node node, std::string attribute_name) {

    std::string attribute = node.get_attribute(attribute_name);
    bool boolean;
    
    try{
        
        boolean = StringTools::to_bool(attribute);
    }catch(std::exception &error) {

        Problem problem(node);

        std::stringstream info;
        info << __FILE__ << ", " << __LINE__ << "\n\n";
        info << "In Xml file: '" << problem.get_path() << ", ";
        info << "line " << problem.get_line() << ", ";
        info << "column " << problem.get_column() << "\n";
        info << "In node '" << node.get_name() << "' ";
        info << "can not parse attribute '" << attribute_name << "' ";
        info << "to bool.\n\n";
        info << problem.get_problem_section_from_original_file() << "\n\n";
        info << error.what() << "\n";
        throw AttributeIsNoBool(info.str());
    }

    return boolean;
}
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
}//Xml