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
using std::string;

namespace Xml {

    string av(const string &att, const string &val);
    string comment(const string &text);
    string to_string(const Vector3D &vec);
    string to_string(const bool b);
}//Xml
#endif // __Xml_AttributeParser_H_INCLUDED__
