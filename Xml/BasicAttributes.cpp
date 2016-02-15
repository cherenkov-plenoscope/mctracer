#include "BasicAttributes.h"
using std::string;

namespace Xml {
//------------------------------------------------------------------------------
string av(const string &att, const string &val) {
    std::stringstream out;
    out << att << "=\"" << val << "\"";
    return out.str();
}
//------------------------------------------------------------------------------
string comment(const string &text) {
    std::stringstream out;
    out << "<!-- " << text << " -->";
    return out.str();
}
//------------------------------------------------------------------------------
string to_string(const Vector3D &vec) {
    std::stringstream out;
    out << "[" << vec.x() << "," << vec.y() << "," <<  vec.z() << "]";
    return out.str();
}
//------------------------------------------------------------------------------
string to_string(const bool b) {
    return (b? "true" : "false");
}
//------------------------------------------------------------------------------
int att2int(Node node, string attribute_name) {

    string attribute = node.attribute(attribute_name);
    int number;
    
    try{
        
        number = StringTools::to_int(attribute);
    }catch(std::exception &error) {

        Problem problem(node);

        std::stringstream info;
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
//------------------------------------------------------------------------------
double att2double(Node node, string attribute_name) {

    string attribute = node.attribute(attribute_name);
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
        info << "In node '" << node.name() << "' ";
        info << "can not parse attribute '" << attribute_name << "' ";
        info << "to double.\n\n";
        info << problem.get_problem_section_from_original_file() << "\n\n";
        info << error.what() << "\n";
        throw AttributeIsNoDouble(info.str());
    }

    return number;
}
//------------------------------------------------------------------------------
double att2bool(Node node, string attribute_name) {

    string attribute = node.attribute(attribute_name);
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
        info << "In node '" << node.name() << "' ";
        info << "can not parse attribute '" << attribute_name << "' ";
        info << "to bool.\n\n";
        info << problem.get_problem_section_from_original_file() << "\n\n";
        info << error.what() << "\n";
        throw AttributeIsNoBool(info.str());
    }

    return boolean;
}
//------------------------------------------------------------------------------
Tuple3 att2Tuple3(Node node, string attribute_name) {

    string attribute = node.attribute(attribute_name);
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
        info << "In node '" << node.name() << "' ";
        info << "can not parse attribute '" << attribute_name << "' ";
        info << "to Tuple3.\n\n";
        info << problem.get_problem_section_from_original_file() << "\n\n";
        info << error.what() << "\n";
        throw AttributeIsNoTuple3(info.str());
    }

    return t3;
}
//------------------------------------------------------------------------------
Vector3D att2Vector3D(Node node, string attribute_name) {

    try{

        Tuple3 t3 = att2Tuple3(node, attribute_name);
        return Vector3D(t3.x, t3.y, t3.z);
    }catch(std::exception &error) {

        std::stringstream info;
        info << "Can not parse to Vector3D.\n";
        info << "\n\n" << error.what() << "\n";
        throw AttributeIsNoVector3D(info.str());
    }
}
//------------------------------------------------------------------------------
Rotation3D att2Rotation3D(Node node, string attribute_name) {

    try{

        Tuple3 t3 = att2Tuple3(node, attribute_name);
        return Rotation3D(t3.x, t3.y, t3.z);
    }catch(std::exception &error) {

        std::stringstream info;
        info << "Can not parse to Rotation3D.\n";
        info << "\n\n" << error.what() << "\n";
        throw AttributeIsNoRotation3D(info.str());
    }
}
//------------------------------------------------------------------------------
Color att2Color(Node node, string attribute_name) {

    try{

        Tuple3 t3 = att2Tuple3(node, attribute_name);
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