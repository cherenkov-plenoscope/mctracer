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

Rot3 FrameFab::fab_rotation(const Xml::Node &node) {	
	if(node.has_attribute("z_reflects_to"))
		return fab_rotation_based_on_z_reflects(node);
	else
		return node.attribute2Rot3("rot");
}

Rot3 FrameFab::fab_rotation_based_on_z_reflects(const Xml::Node &node) {
	Vec3 focal_point = node.attribute2Vec3("z_reflects_to");
	Vec3 focal_point_to_pos = focal_point - pos;
	Vec3 rotation_axis = focal_point_to_pos.cross(Vec3::unit_z);
	double rot_angle = -0.5 * Vec3::unit_z.get_angle_in_between_in_rad(
		focal_point_to_pos
	);
	return Rot3(rotation_axis, rot_angle);
}

Vec3 FrameFab::fab_position(const Xml::Node &node) {	
	return node.attribute2Vec3("pos");
}

std::string FrameFab::fab_name(const Xml::Node &node) {	
	return node.attribute("name");	
}