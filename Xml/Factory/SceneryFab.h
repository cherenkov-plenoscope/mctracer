//=================================
// include guard
#ifndef __Xml_Factory_H_INCLUDED__
#define __Xml_Factory_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Xml/Xml.h"
#include "FunctionFab.h"

namespace Xml {

struct Fab {

   	FunctionFab functions;
    Frame world;

    Xml::Node node;

    Fab(const std::string path) {

        Xml::Document doc(path);
        node = doc.get_tree();
    };
};

}//Xml
#endif // __Xml_Factory_H_INCLUDED__
