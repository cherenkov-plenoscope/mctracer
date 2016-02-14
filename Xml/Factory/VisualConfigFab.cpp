#include "VisualConfigFab.h"
namespace Xml {
	namespace Configs {
//------------------------------------------------------------------------------
VisualConfig get_VisualConfig_from_node(Xml::Node node) {

	VisualConfig vc;

	Xml::Node preview = node.child("preview");
	vc.preview.cols = Xml::att2int(preview, "cols");
	vc.preview.rows = Xml::att2int(preview, "rows");
	vc.preview.scale = Xml::att2int(preview, "scale");

	Xml::Node snapshot = node.child("snapshot");
	vc.snapshot.cols = Xml::att2int(snapshot, "cols");
	vc.snapshot.rows = Xml::att2int(snapshot, "rows");
	vc.snapshot.rays_per_pixel = Xml::att2int(snapshot, "rays_per_pixel");
	vc.snapshot.focal_length_over_aperture_diameter = Xml::att2double(snapshot, "focal_length_over_aperture_diameter");
	vc.snapshot.image_sensor_size_along_a_row = Xml::att2double(snapshot, "image_sensor_size_along_a_row");

	Xml::Node global_illumination = node.child("global_illumination");
	vc.global_illumination.on = Xml::att2bool(global_illumination, "on");
	vc.global_illumination.incoming_direction = Xml::att2Vector3D(global_illumination, "incoming_direction");

	Xml::Node photon_trajectories = node.child("photon_trajectories");
	vc.photon_trajectories.radius = Xml::att2double(photon_trajectories, "radius");

	Xml::Node sky_dome_node = node.child("sky_dome");
	std::string image_path = sky_dome_node.attribute("image_path");
	

	if(image_path.empty()) {

		vc.sky_dome = SkyDome(Xml::att2Color(sky_dome_node, "color"));
	}else{

		vc.sky_dome = SkyDome(image_path);
		vc.sky_dome.set_background_color(
			Xml::att2Color(sky_dome_node, "color")
		);
	}

	return vc;
}
//------------------------------------------------------------------------------
std::string to_node(const VisualConfig &vc) {

	std::stringstream out;
	out << "<visual>\n";

	out << "    " << comment(
	"Image size of the interactive preview window (cols*scale x rows*scale). "
	"For fast preview, the initial image is only (cols x rows) and on demand a full resolution image (cols*scale x rows*scale) is created.") << "\n";
	out << "	<preview " << av("cols", std::to_string(vc.preview.cols)) << " ";
	out                    << av("rows", std::to_string(vc.preview.rows)) << " ";
	out                    << av("scale", std::to_string(vc.preview.scale)) << "/>\n";

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