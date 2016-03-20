//=================================
// include guard
#ifndef __SceneryFactory_H_INCLUDED__
#define __SceneryFactory_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Xml/Xml.h"
#include "Xml/Factory/FunctionFab.h"
#include "Tools/PathTools.h"
#include "Core/Frame.h"
using std::string;

//------------------------------------------------------------------------------
namespace Xml {

    class SceneryFactory {
    public:

        Frame* scenery;
        FunctionFab functions;
        PathTools::Path xml_path;
        Document xml_doc;

        SceneryFactory(const string path);
    private:
        void throw_unkonown_key(const Node node);
        void make_geometry(Frame* mother, const Node node);
        Color* color(const Node node)const;
        Frame* add_Frame(Frame* mother, const Node node);
        Frame* add_Disc(Frame* mother, const Node node);
    };
}//Xml
#endif // __SceneryFactory_H_INCLUDED__ 