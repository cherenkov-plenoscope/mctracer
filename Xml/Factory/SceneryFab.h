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
#include "FrameFab.h"

namespace Xml {

struct Fab {

   	FunctionFab functions;
    Frame world;

    Xml::Node node;

    Fab(const std::string path) {

        Xml::Document doc(path);
        node = doc.get_tree();
    };

	Frame* fab_Frame(Frame* mother,	const Xml::Node &node) {
		
		FrameFab fab(node);
		Frame *frame = new Frame(fab.name, fab.pos, fab.rot);
		mother->set_mother_and_child(frame);
		return frame;
	}

	Frame* fab_Plane(Frame* mother,	const Xml::Node &node) {
		Framefab frame_fab(node);

		const Color* = new Color(Xml::attribute_as_Color(node, "color"));
		const Function::Func1D* reflection_vs_wavelength
		
		Xml::Node surface = node.get_child("set_surface");
		Xml::Node set_plane = node.get_child("set_plane");

		Plane *plane = new Plane(frame_fab.name, frame_fab.pos, frame_fab.rot);
		plane->set_inner_color(color);
		plane->set_outer_color(color);
		plane->set_outer_reflection(functions.by_name(surface.get_attribute("")));
		plane->set_inner_reflection(functions.by_name(surface.get_attribute("")));
		plane->set_x_y_width(
			Xml::attribute_as_double(set_plane, "x_width"),
			Xml::attribute_as_double(set_plane, "y_width"),
		);
		mother->set_mother_and_child(plane);
		return plane;
	}
};

}//Xml
#endif // __Xml_Factory_H_INCLUDED__