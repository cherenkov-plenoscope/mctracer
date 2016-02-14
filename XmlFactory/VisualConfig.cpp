#include "VisualConfig.h"
namespace Xml {
	namespace Configs {
//------------------------------------------------------------------------------
VisualConfig get_VisualConfig_from_node(Xml::Node node) {

	VisualConfig vc;

	Xml::Node preview = node.get_child("preview");
	vc.preview.cols = Xml::attribute_to_double(preview, "cols");
	vc.preview.rows = Xml::attribute_to_double(preview, "rows");

	Xml::Node snapshot = node.get_child("snapshot");
	vc.snapshot.cols = Xml::attribute_to_double(snapshot, "cols");
	vc.snapshot.rows = Xml::attribute_to_double(snapshot, "rows");
	vc.snapshot.rays_per_pixel = Xml::attribute_to_double(snapshot, "rays_per_pixel");
	vc.snapshot.focal_length_over_aperture_diameter = Xml::attribute_to_double(snapshot, "focal_length_over_aperture_diameter");
	vc.snapshot.image_sensor_size_along_a_row = Xml::attribute_to_double(snapshot, "image_sensor_size_along_a_row");

	Xml::Node global_illumination = node.get_child("global_illumination");
	vc.global_illumination.on = Xml::attribute_to_bool(global_illumination, "on");
	vc.global_illumination.incoming_direction = Xml::attribute_to_Vector3D(global_illumination, "incoming_direction");

	Xml::Node photon_trajectories = node.get_child("photon_trajectories");
	vc.photon_trajectories.radius = Xml::attribute_to_double(photon_trajectories, "radius");

	Xml::Node sky_dome_node = node.get_child("sky_dome");
	std::string image_path = sky_dome_node.get_attribute("image_path");
	

	if(image_path.empty()) {

		vc.sky_dome = SkyDome(Xml::attribute_to_Color(sky_dome_node, "color"));
	}else{

		vc.sky_dome = SkyDome(image_path);
		vc.sky_dome.set_background_color(
			Xml::attribute_to_Color(sky_dome_node, "color")
		);
	}

	return vc;
}
//------------------------------------------------------------------------------
std::string to_node(const VisualConfig &vc) {

	std::stringstream out;
	out << "<visual>\n";

	out << "    " << comment(
	"Image size of the interactive preview window. "
	"Large images take more time. "
	"A single ray is propagated for each of the (rows x cols) pixels.") << "\n";
	out << "	<preview " << av("cols", std::to_string(vc.preview.cols)) << " ";
	out                    << av("rows", std::to_string(vc.preview.rows)) << "/>\n";

	out << "    " << comment(
	"Snaphot images are taken with a cineatic camera with depth of field. "
	"Several rays are emitted per pixel (rows x cols x rays_per_pixel). "
	"The more rays per pixel are emitted, the less noisy the image's depth of "
	"field will be, but the longer it takes to be calculated. "
	"For a strong depth of field, choose a small focal_length_over_aperture_diameter, "
	"e.g. 0.95, and a large sensor size e.g. 70mm. " 
	"For a moderate human like depth of field, choose focal_length_over_aperture_diameter=2.8 and image_sensor_size_along_a_row=25mm.") << "\n";
	
	out << "	<snapshot " << av("cols", std::to_string(vc.snapshot.cols)) << " ";
	out                     << av("rows", std::to_string(vc.snapshot.rows)) << " ";
	out                     << av("rays_per_pixel", std::to_string(vc.snapshot.rays_per_pixel)) << " ";
	out                     << av("focal_length_over_aperture_diameter", std::to_string(vc.snapshot.focal_length_over_aperture_diameter)) << " ";
	out                     << av("image_sensor_size_along_a_row", std::to_string(vc.snapshot.image_sensor_size_along_a_row)) << "/>\n";

	out << "	<global_illumination " 	<< av("on", to_string(vc.global_illumination.on)) << " ";
	out                     			<< av("incoming_direction", to_string(vc.global_illumination.incoming_direction)) << "/>\n";

	out << "	<photon_trajectories " 	<< av("radius", std::to_string(vc.photon_trajectories.radius)) << "/>\n";

	out << "    " << comment(
	"A sky dome is the background to be seen in a scenery. "
	"A textured sky dome image helps the 3D expirience. "
	"Best choose a sky dome image taken with a circular fisheye optics. "
	"If the image_path is empty, the sky dome will be monochrome with the "
	"color defined here. If the sky dome is taken from a file, the color "
	"is used below the horizon.") << "\n";

	out << "	<sky_dome " << av("image_path", "") << " ";
	out            			<< av("color", "[255,255,255]") <<"/>\n";

	out << "</visual>\n";
	return out.str();
}
//------------------------------------------------------------------------------
	}
}