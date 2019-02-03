// Copyright 2014 Sebastian A. Mueller
#ifndef XML_FACTORY_SCENERYFACTORY_H_
#define XML_FACTORY_SCENERYFACTORY_H_

#include <vector>
#include <string>
#include "Xml/Xml.h"
#include "Tools/PathTools.h"
#include "Core/Frame.h"
#include "Plenoscope/PlenoscopeInScenery.h"
#include "Scenery/Scenery.h"
#include "Xml/Factory/FrameFab.h"
#include "Scenery/Primitive/Primitive.h"
#include "Scenery/StereoLitography/StereoLitography.h"
#include "Scenery/SegmentedReflector/SegmentedReflector.h"

namespace Xml {

class SceneryFactory {
 public:
    std::string author;
    std::string comment;
    std::vector<Plenoscope::PlenoscopeInScenery> plenoscopes;
    PathTools::Path xml_path;
    Document xml_doc;
    Scenery* scenery;
    explicit SceneryFactory(const std::string path);
    void append_to_frame_in_scenery(Frame* frame, Scenery* scenery);

 private:
    void make_geometry(Frame* mother, const Node node);
};

void add_sensor(Frame* frame, const Node node, Scenery* scenery);
void add_color(const Node node, Scenery* scenery);
void add_function(const Node node, Scenery* scenery);
const Function::Func1D* surface_refl(const Node node, const Scenery* scenery);
const Function::Func1D* surface_refrac(const Node node, const Scenery* scenery);
const Color* surface_color(const Node node, Scenery* scenery);
Frame* add_Frame(Frame* mother, const Node node);
Frame* add_Disc(Frame* mother, const Node node, Scenery *scenery);
Frame* add_Sphere(Frame* mother, const Node node, Scenery *scenery);
Frame* add_Plane(Frame* mother, const Node node, Scenery *scenery);
Frame* add_HexPlane(Frame* mother, const Node node, Scenery *scenery);
Frame* add_Cylinder(Frame* mother, const Node node, Scenery *scenery);
Frame* add_Annulus(Frame* mother, const Node node, Scenery *scenery);
Frame* add_BiConvexLensHex(Frame* mother, const Node node, Scenery *scenery);
Frame* add_Triangle(Frame* mother, const Node node, Scenery *scenery);
Frame* add_SphereCapWithHexagonalBound(
    Frame* mother,
    const Node node,
    Scenery* scenery);
Frame* add_SphereCapWithRectangularBound(
    Frame* mother,
    const Node node,
    Scenery* scenery);
Frame* add_SegmentedReflector(Frame* mother, const Node node, Scenery* scenery);
Frame* add_STL(Frame* mother, const Node node, Scenery *scenery);
Frame* add_light_field_sensor(
    Frame* mother,
    const Node node,
    Scenery* scenery,
    std::vector<Plenoscope::PlenoscopeInScenery>* plenoscopes);
Frame* add_light_field_sensor_demonstration(
    Frame* mother,
    const Node node,
    Scenery* scenery,
    std::vector<Plenoscope::PlenoscopeInScenery>* plenoscopes);

}  // namespace Xml

#endif  // XML_FACTORY_SCENERYFACTORY_H_
