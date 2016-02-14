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
#include "Tools/StringTools.h"

namespace Xml {

    std::string av(const std::string &att, const std::string &val);
    std::string comment(const std::string &text);
    std::string to_string(const Vector3D &vec);
    std::string to_string(const bool b);

    class AttributeIsNoInt : public TracerException {
        using TracerException::TracerException;
    }; 
    int att2int(Node node, std::string attribute_name);

    class AttributeIsNoDouble : public TracerException {
        using TracerException::TracerException;
    }; 
    double att2double(Node node, std::string attribute_name);

    class AttributeIsNoBool : public TracerException {
        using TracerException::TracerException;
    }; 
    double att2bool(Node node, std::string attribute_name);

    class AttributeIsNoTuple3 : public TracerException {
        using TracerException::TracerException;
    }; 
    Tuple3 att2Tuple3(Node node, std::string attribute_name);

    class AttributeIsNoVector3D : public TracerException {
        using TracerException::TracerException;
    }; 
    Vector3D att2Vector3D(Node node, std::string attribute_name);
    
    class AttributeIsNoRotation3D : public TracerException {
        using TracerException::TracerException;
    }; 
    Rotation3D att2Rotation3D(Node node, std::string attribute_name);
    
    class AttributeIsNoColor : public TracerException {
        using TracerException::TracerException;
    }; 
    Color att2Color(Node node, std::string attribute_name);
}//Xml
#endif // __Xml_AttributeParser_H_INCLUDED__
