#include "Rotation3DFactory.h"
#include "Vector3DFactory.h"
//------------------------------------------------------------------------------
Rotation3DFactory::Rotation3DFactory(const std::string name ,const pugi::xml_node node) {
	
	assert_attribute_exists(node, name);	
	rot = extract_frmae_rotation_xyz_angles(name, node);
}
//------------------------------------------------------------------------------
Rotation3D Rotation3DFactory::get_rot()const {
	return rot;
}
//------------------------------------------------------------------------------
Rotation3D Rotation3DFactory::extract_frmae_rotation_xyz_angles(
	const std::string name, const pugi::xml_node node
) {
	assert_attribute_exists(node, name);	
	double rX, rY, rZ;
	strto3tuple(rX, rY, rZ, node.attribute(name.c_str()).value());
	return Rotation3D(rX, rY, rZ);		
}