#include "SceneryFactory.h"
#include "FrameFab.h"
#include "Geometry/Disc.h"
#include "Geometry/Sphere.h"
using StringTools::is_equal;
using std::stringstream;

//------------------------------------------------------------------------------
namespace Xml {
SceneryFactory::SceneryFactory(const string path): xml_path(path), xml_doc(path) {

    const Node root_node = xml_doc.node().first_child();
    
    if(root_node.has_attribute("author"))
        author = root_node.attribute("author");
    if(root_node.has_attribute("comment"))
        comment = root_node.attribute("comment");

    telescopes = new TelescopeArrayControl();
    sensors = new std::vector<PhotonSensor::Sensor*>;

    scenery = new Frame("scenery", Vector3D::null, Rotation3D::null);
    make_geometry(scenery, root_node);
    scenery->init_tree_based_on_mother_child_relations();
}
//------------------------------------------------------------------------------
void SceneryFactory::make_geometry(Frame* mother, const Node node) {

    for(Node child=node.first_child(); child; child=child.next_child()) {

        std::cout << child.name() << "\n";
        if(is_equal(child.name(), "function"))
            functions.add(child);
        else if(is_equal(child.name(), "frame"))
            make_geometry(add_Frame(mother, child), child);
        else if(is_equal(child.name(), "disc"))
            make_geometry(add_Disc(mother, child), child);
        else if(is_equal(child.name(), "sphere"))
            make_geometry(add_Sphere(mother, child), child);
    }
}
//------------------------------------------------------------------------------
void SceneryFactory::throw_unkonown_key(const Node node) {

    Problem xml_problem(node);
    stringstream info;
    info << __FILE__ << ", " << __LINE__ << "\n";
    info << "SceneryFactory::" << __func__ << "()\n";
    info << "Unknown name: '" << node.name() << "'\n";
    info << "In xml file: " << xml_problem.get_path();
    info << ", (" << xml_problem.get_line();
    info << ", " << xml_problem.get_column() << ")\n";
    info << xml_problem.get_problem_section_from_original_file();
    throw TracerException(info.str());
}
//------------------------------------------------------------------------------
Frame* SceneryFactory::add_Frame(Frame* mother, const Node node) {

    FrameFab fab(node);
    Frame* frame = new Frame(fab.name, fab.pos, fab.rot);
    mother->set_mother_and_child(frame);
    return frame;
}
//------------------------------------------------------------------------------
Frame* SceneryFactory::add_Disc(Frame* mother, const Node node) {

    FrameFab framefab(node);
    Disc* disc = new Disc;
    disc->set_name_pos_rot(framefab.name, framefab.pos, framefab.rot);

    disc->set_inner_color(color(node.child("set_surface")));
    disc->set_outer_color(color(node.child("set_surface")));
    disc->set_outer_reflection(functions.by_name(node.child("set_surface").attribute("reflection_vs_wavelength")));
    disc->set_inner_reflection(functions.by_name(node.child("set_surface").attribute("reflection_vs_wavelength"))); 
    disc->set_radius(node.child("set_disc").attribute2double("radius"));
    mother->set_mother_and_child(disc);
    return disc;
}
//------------------------------------------------------------------------------
Frame* SceneryFactory::add_Sphere(Frame* mother, const Node node) {

    FrameFab framefab(node);
    Sphere* sphere = new Sphere;
    sphere->set_name_pos_rot(framefab.name, framefab.pos, framefab.rot);

    sphere->set_inner_color(color(node.child("set_surface")));
    sphere->set_outer_color(color(node.child("set_surface")));
    sphere->set_outer_reflection(functions.by_name(node.child("set_surface").attribute("reflection_vs_wavelength")));
    sphere->set_inner_reflection(functions.by_name(node.child("set_surface").attribute("reflection_vs_wavelength")));   
    sphere->set_sphere_radius(node.child("set_sphere").attribute2double("radius"));
    mother->set_mother_and_child(sphere);
    return sphere;
}
//------------------------------------------------------------------------------
Color* SceneryFactory::color(const Node node)const {
    Color* color = new Color(node.attribute2Color("color"));
    return color;
}
//------------------------------------------------------------------------------
}//Xml