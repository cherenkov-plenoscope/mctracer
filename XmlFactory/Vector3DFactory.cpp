#include "Vector3DFactory.h"
//------------------------------------------------------------------------------
Vector3DFactory::Vector3DFactory(const std::string name ,const pugi::xml_node node) {
	
	assert_attribute_exists(node, name);
	double pX, pY, pZ;
	strto3tuple(pX, pY, pZ, node.attribute(name.c_str()).value());
	vec = Vector3D(pX, pY, pZ);
}
//------------------------------------------------------------------------------
Vector3D Vector3DFactory::get_vec()const {
	return vec;
}