#include "VisualConfigFab.h"
#include "Tools/PathTools.h"
#include <sstream>

namespace Xml {
	namespace Configs {
//------------------------------------------------------------------------------
Visual::Config get_VisualConfig_from_node(Xml::Node node) {

	Visual::Config vc;

	vc.max_interaction_depth = node.attribute2int("max_interaction_depth");

	Xml::Node preview = node.child("preview");
	vc.preview.cols = preview.attribute2int("cols");
	vc.preview.rows = preview.attribute2int("rows");
	vc.preview.scale = preview.attribute2int("scale");

	Xml::Node snapshot = node.child("snapshot");
	vc.snapshot.cols = snapshot.attribute2int("cols");
	vc.snapshot.rows = snapshot.attribute2int("rows");
	vc.snapshot.noise_level = snapshot.attribute2int("noise_level");
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

		vc.sky_dome = Visual::SkyDome(sky_dome_node.attribute2Color("color"));
	}else{

		PathTools::Path xmlpath = PathTools::Path(node.xml_path());
		vc.sky_dome = Visual::SkyDome(PathTools::join(xmlpath.dirname, image_path));
		vc.sky_dome.set_background_color(
			sky_dome_node.attribute2Color("color")
		);
	}

	return vc;
}
//------------------------------------------------------------------------------
std::string to_node(const Visual::Config &vc) {

	std::stringstream out;
	out << "<visual " << av("max_interaction_depth", std::to_string(vc.max_interaction_depth)) << ">\n";

	out << "    " << comment(
    "Image size of the interactive preview window (cols*scale x rows*scale).\n"
    "For fast preview, the initial image is only (cols x rows) and on\n"
    "demand a full resolution image (cols*scale x rows*scale) is created.\n"
    ) << "\n";
	out << "	<preview " << av("cols", std::to_string(vc.preview.cols)) << " ";
	out                    << av("rows", std::to_string(vc.preview.rows)) << " ";
	out                    << av("scale", std::to_string(vc.preview.scale)) << "/>\n";

	out << "    " << comment(
    "Snaphot images are taken with a cinematic camera with an extended\n"
    "aperture and depth-of-field.\n"
    "Choose a low noise_level to obtain smooth images, choose a\n"
    "high noise_level for fast imaging. The noise_level is an 8bit unsigned\n"
    "integer between 0 and 255.\n") << "\n";

	out << "	<snapshot " << av("cols", std::to_string(vc.snapshot.cols)) << " ";
	out                     << av("rows", std::to_string(vc.snapshot.rows)) << " ";
	out                     << av("noise_level", std::to_string(vc.snapshot.noise_level)) << " ";
	out                     << av("focal_length_over_aperture_diameter", std::to_string(vc.snapshot.focal_length_over_aperture_diameter)) << " ";
	out                     << av("image_sensor_size_along_a_row", std::to_string(vc.snapshot.image_sensor_size_along_a_row)) << "/>\n";

	out << "	<global_illumination " 	<< av("on", to_string(vc.global_illumination.on)) << " ";
	out                     			<< av("incoming_direction", to_string(vc.global_illumination.incoming_direction)) << "/>\n";

	out << "	<photon_trajectories " 	<< av("radius", std::to_string(vc.photon_trajectories.radius)) << "/>\n";

	out << "    " << comment(
    "A sky dome is the background to be seen in a scenery. A textured\n"
    "sky dome image helps the 3D expirience. Best choose a sky dome image\n"
    "taken with a circular fisheye optics. If the image_path is empty,\n"
    "the sky dome will be monochrome with the color defined here. If the\n"
    "sky dome is taken from a file, the color is used below the horizon.\n"
	) << "\n";

	out << "	<sky_dome " << av("image_path", "") << " ";
	out            			<< av("color", "[255,255,255]") <<"/>\n";

	out << "</visual>\n";
	return out.str();
}
//------------------------------------------------------------------------------
	}
}