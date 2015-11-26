#include "FrameFactory.h"
#include "Vector3DFactory.h"
#include "Rotation3DFactory.h"
//------------------------------------------------------------------------------
FrameFactory::FrameFactory(const pugi::xml_node node) {
	
	assert_child_exists(node, "set_frame");
	extract_frame(node.child("set_frame"));
} 
//------------------------------------------------------------------------------
void FrameFactory::extract_frame(const pugi::xml_node node) {

	name = extract_frame_name(node);
	position = extract_frame_position(node);
	rotation = extract_frame_rotation(node);
}
//------------------------------------------------------------------------------
Rotation3D FrameFactory::extract_frame_rotation(const pugi::xml_node node) {	
	
	if(has_attribute(node, "z_reflects_to"))
		return extract_frame_rotation_based_on_z_reflects(node);
	else
		return extract_frame_rotation_xyz_angles(node);
}
//------------------------------------------------------------------------------
Rotation3D FrameFactory::extract_frame_rotation_based_on_z_reflects(
	const pugi::xml_node node
) {

	Vector3DFactory vec_fab("z_reflects_to", node);
	Vector3D focal_point = vec_fab.get_vec();

	Vector3D focal_point_to_pos = focal_point - position;
	Vector3D rotation_axis = focal_point_to_pos.cross(Vector3D::unit_z);

	double rot_angle = -0.5 * Vector3D::unit_z.get_angle_in_between_in_rad(
		focal_point_to_pos
	);

	return Rotation3D(rotation_axis, rot_angle);
}
//------------------------------------------------------------------------------
Rotation3D FrameFactory::extract_frame_rotation_xyz_angles(
	const pugi::xml_node node
) {
	Rotation3DFactory rot_fab("rot", node);
	return rot_fab.get_rot();	
}
//------------------------------------------------------------------------------
Vector3D FrameFactory::extract_frame_position(const pugi::xml_node node) {	
	
	Vector3DFactory vecFab("pos", node);
	return vecFab.get_vec();		
}
//------------------------------------------------------------------------------
std::string FrameFactory::extract_frame_name(const pugi::xml_node node) {	
	
	assert_attribute_exists(node, "name");
	return node.attribute("name").value();		
}
//------------------------------------------------------------------------------
std::string FrameFactory::get_name()const {return name;}
//------------------------------------------------------------------------------
Vector3D FrameFactory::get_position()const {return position;}
//------------------------------------------------------------------------------
Rotation3D FrameFactory::get_rotation()const {return rotation;}