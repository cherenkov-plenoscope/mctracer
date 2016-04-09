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
#include "PhotonSensor/PhotonSensor.h"
#include "TelescopeArrayControl.h"
#include <map>
using std::string;
using std::vector;
using std::map;

//------------------------------------------------------------------------------
namespace Xml {

    class SceneryFactory {
    public:

        Frame* scenery;

        vector<PhotonSensor::Sensor*>* raw_sensors;
        FunctionFab functions;
        TelescopeArrayControl* telescopes;

        string author;
        string comment;

        PathTools::Path xml_path;
        Document xml_doc;

        SceneryFactory(const string path);
        PhotonSensors::Sensors sensors()const;
    private:

        void make_geometry(Frame* mother, const Node node);
        Frame* add_Frame(Frame* mother, const Node node);
        Frame* add_Disc(Frame* mother, const Node node);
        Frame* add_Sphere(Frame* mother, const Node node);
        Frame* add_Plane(Frame* mother, const Node node);
        Frame* add_HexPlane(Frame* mother, const Node node);
        Frame* add_Cylinder(Frame* mother, const Node node);
        Frame* add_Annulus(Frame* mother, const Node node);
        Frame* add_BiConvexLensHex(Frame* mother, const Node node);
        Frame* add_SphereCapWithHexagonalBound(Frame* mother, const Node node);
        Frame* add_Triangle(Frame* mother, const Node node);
        Frame* add_SegmentedReflector(Frame* mother, const Node node);
        Frame* add_STL(Frame* mother, const Node node);
        Frame* add_Plenoscope(Frame* mother, const Node node);

        Color* surface_color(const Node node)const;
        const Function::Func1D* surface_refl(const Node node)const;
        void add_to_sensors_if_sensitive(Frame* frame, const Node node);
        void add_to_array_if_telescope(Frame* frame, const Node node);
    };
}//Xml
#endif // __SceneryFactory_H_INCLUDED__ 