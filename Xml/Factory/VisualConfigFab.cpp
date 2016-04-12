#include "VisualConfigFab.h"
#include "Tools/PathTools.h"
namespace Xml {
	namespace Configs {
//------------------------------------------------------------------------------
VisualConfig get_VisualConfig_from_node(Xml::Node node) {

	VisualConfig vc;

	Xml::Node preview = node.child("preview");
	vc.preview.cols = preview.attribute2int("cols");
	vc.preview.rows = preview.attribute2int("rows");
	vc.preview.scale = preview.attribute2int("scale");

	Xml::Node snapshot = node.child("snapshot");
	vc.snapshot.cols = snapshot.attribute2int("cols");
	vc.snapshot.rows = snapshot.attribute2int("rows");
	vc.snapshot.rays_per_pixel = snapshot.attribute2int("rays_per_pixel");
	vc.snapshot.focal_length_over_aperture_diameter = snapshot.attribute2double("focal_length_over_aperture_diameter");
	vc.snapshot.image_sensor_size_along_a_row = snapshot.attribute2double("image_sensor_size_along_a_row");

	Xml::Node global_illumination = node.child("global_illumination");
	vc.global_illumination.on = global_illumination.attribute2bool("on");
	vc.global_illumination.incoming_direction = global_illumination.attribute2Vec3("incoming_direction");

	Xml::Node photon_trajectories = node.child("photon_trajectories");
	vc.photon_trajectories.radius = photon_trajectories.attribute2double("radius");

	Xml::Node sky_dome_node = node.child("sky_dome");
	std::string image_path = sky_dome_node.attribute("image_path");
	

	if(image_path.empty()) {

		vc.sky_dome = SkyDome(sky_dome_node.attribute2Color("color"));
	}else{
		PathTools::FullPath xmlpath = PathTools::split_path_and_filename(node.xml_path());

		vc.sky_dome = SkyDome(xmlpath.path+"/"+image_path);
		vc.sky_dome.set_background_color(
			sky_dome_node.attribute2Color("color")
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