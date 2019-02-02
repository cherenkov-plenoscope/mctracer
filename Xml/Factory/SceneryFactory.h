// Copyright 2014 Sebastian A. Mueller
#ifndef XML_FACTORY_SCENERYFACTORY_H_
#define XML_FACTORY_SCENERYFACTORY_H_

#include <vector>
#include <string>
#include "Xml/Xml.h"
#include "Tools/PathTools.h"
#include "Core/Frame.h"
#include "PhotonSensor/PhotonSensor.h"
#include "Plenoscope/PlenoscopeInScenery.h"
#include "Scenery/Scenery.h"
#include "Xml/Factory/FrameFab.h"

namespace Xml {

class SceneryFactory {
 public:
    std::vector<PhotonSensor::Sensor*>* raw_sensors;
    std::vector<Plenoscope::PlenoscopeInScenery> plenoscopes;

    std::string author;
    std::string comment;

    PathTools::Path xml_path;
    Document xml_doc;

    Scenery* scenery;

    explicit SceneryFactory(const std::string path);
    PhotonSensor::Sensors sensors()const;
    void append_to_frame_in_scenery(Frame* frame, Scenery* scenery);

 private:
    void make_geometry(Frame* mother, const Node node);
    Frame* add_Disc(Frame* mother, const Node node);
    Frame* add_Sphere(Frame* mother, const Node node);
    Frame* add_Plane(Frame* mother, const Node node);
    Frame* add_HexPlane(Frame* mother, const Node node);
    Frame* add_Cylinder(Frame* mother, const Node node);
    Frame* add_Annulus(Frame* mother, const Node node);
    Frame* add_BiConvexLensHex(Frame* mother, const Node node);
    Frame* add_SphereCapWithHexagonalBound(Frame* mother, const Node node);
    Frame* add_SphereCapWithRectangularBound(Frame* mother, const Node node);
    Frame* add_Triangle(Frame* mother, const Node node);
    Frame* add_SegmentedReflector(Frame* mother, const Node node);
    Frame* add_STL(Frame* mother, const Node node);
    Frame* add_light_field_sensor(Frame* mother, const Node node);
    Frame* add_light_field_sensor_demonstration(Frame* mother, const Node node);

    const Color* surface_color(const Node node)const;
    const Function::Func1D* surface_refl(const Node node)const;
    const Function::Func1D* surface_refrac(const Node node)const;
    void add_to_sensors_if_sensitive(Frame* frame, const Node node);
    void add_to_array_if_telescope(Frame* frame, const Node node);

    void add_function(const Node node);
    void add_color(const Node node);
};

Frame* add_Frame(Frame* mother, const Node node) {
    FrameFab fab(node);
    Frame* frame = mother->append<Frame>();
    frame->set_name_pos_rot(fab.name, fab.pos, fab.rot);
    return frame;
}

}  // namespace Xml

#endif  // XML_FACTORY_SCENERYFACTORY_H_
