#include "SceneryFactory.h"
#include "FrameFab.h"
#include "Geometry/Disc.h"
using StringTools::is_equal;
using std::stringstream;

//------------------------------------------------------------------------------
namespace Xml {
SceneryFactory::SceneryFactory(const string path): xml_path(path), xml_doc(path) {

	const Node root_node = xml_doc.node().first_child();
	scenery = new Frame(root_node.name(), Vector3D::null, Rotation3D::null);
	make_geometry(scenery, root_node);
}
//------------------------------------------------------------------------------
void SceneryFactory::make_geometry(Frame* mother, const Node node) {

	for(Node child=node.first_child(); child; child=child.next_child()) {

		if(is_equal(child.name(), "function"))
			functions.add(child);
		else if(is_equal(child.name(), "frame"))
			mother = add_Frame(mother, child);
		else if(is_equal(child.name(), "disc"))
			mother = add_Disc(mother, child);
		else
			throw_unkonown_key(child);
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
Color* SceneryFactory::color(const Node node)const {
	Color* color = new Color(node.attribute2Color("color"));
	return color;
}
//------------------------------------------------------------------------------
}//Xml