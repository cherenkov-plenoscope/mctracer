#include "LightFieldTelescopeFactory.h"
#include "Tools/StringTools.h"
#include "Tools/Tools.h"

//------------------------------------------------------------------------------
LightFieldTelescopeFactory::LightFieldTelescopeFactory(const pugi::xml_node newnode):node(newnode) {
} 

//------------------------------------------------------------------------------
double LightFieldTelescopeFactory::get_max_FoV_diameter(){
	assert_attribute_exists(node, "max_FoV_diameter_deg");
	return Deg2Rad(StrToDouble(node.attribute("max_FoV_diameter_deg").value()));
}
//------------------------------------------------------------------------------
double LightFieldTelescopeFactory::get_hex_pixel_FoV_flat2flat(){
	assert_attribute_exists(node, "hex_pixel_FoV_flat2flat_deg");
	return Deg2Rad(StrToDouble(node.attribute("hex_pixel_FoV_flat2flat_deg").value()));
}
//------------------------------------------------------------------------------
double LightFieldTelescopeFactory::get_housing_overhead(){
	assert_attribute_exists(node, "housing_overhead");
	return StrToDouble(node.attribute("housing_overhead").value());
}
//------------------------------------------------------------------------------
double LightFieldTelescopeFactory::get_sub_pixel_per_pixel(){
	assert_attribute_exists(node, "sub_pixel_per_pixel");
	return StrToDouble(node.attribute("sub_pixel_per_pixel").value());
}
//------------------------------------------------------------------------------
std::string LightFieldTelescopeFactory::get_refraction_vs_wavelength(){
	assert_attribute_exists(node, "refraction_vs_wavelength");
	return node.attribute("refraction_vs_wavelength").value();
}
