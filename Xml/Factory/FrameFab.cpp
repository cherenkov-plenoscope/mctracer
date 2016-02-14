#include "FrameFab.h"

FrameFab::FrameFab(const Xml::Node &node) {
	Xml::Node set_frame = node.child("set_frame");
	fab_frame(set_frame);
} 

void FrameFab::fab_frame(const Xml::Node &node) {
	name = fab_name(node);
	pos = fab_position(node);
	rot = fab_rotation(node);
}

Rotation3D FrameFab::fab_rotation(const Xml::Node &node) {	
	if(node.has_attribute("z_reflects_to"))
		return fab_rotation_based_on_z_reflects(node);
	else
		return Xml::att2Rotation3D(node, "rot");
}

Rotation3D FrameFab::fab_rotation_based_on_z_reflects(const Xml::Node &node) {
	Vector3D focal_point = Xml::att2Vector3D(node, "z_reflects_to");
	Vector3D focal_point_to_pos = focal_point - pos;
	Vector3D rotation_axis = focal_point_to_pos.cross(Vector3D::unit_z);
	double rot_angle = -0.5 * Vector3D::unit_z.get_angle_in_between_in_rad(
		focal_point_to_pos
	);
	return Rotation3D(rotation_axis, rot_angle);
}

Vector3D FrameFab::fab_position(const Xml::Node &node) {	
	return Xml::att2Vector3D(node, "pos");
}

std::string FrameFab::fab_name(const Xml::Node &node) {	
	return node.attribute("name");	
}