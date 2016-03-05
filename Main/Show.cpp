#include "Core/Photons.h"
#include "XmlFactory/WorldFactory.h"
#include "Geometry/StereoLitographyIo/StereoLitographyIo.h"
#include "Xml/Factory/VisualConfigFab.h"
#include "Xml/Factory/TracerSettingsFab.h"
#include "CommandLine/CommandLine.h"
#include "Cameras/FlyingCamera.h"
using std::string;
using std::cout;

string help_text() {
	std::stringstream out; 
	out << "  Explore a scenery in 1st person view\n";
	out << "  --scenery, -s        path of scenery\n";
	out << "  --visual_config, -v  [optional]\n";
	return out.str();	
}

int main(int argc, char* argv[]) {
	try{
		
	CommandLine::Parser cmd;
	cmd.define_key_val_by_key_short_desc("scenery", 's', "scenery path");
	cmd.define_key_val_by_key_short_desc("visual_config", 'v' ,"visual config path");
	cmd.parse(argc, argv);

	if(!cmd.exist("scenery")) {
		cout << help_text();
		return 0;
	}

	Frame *scenery;
	if(
		StringTools::is_ending(cmd.get("scenery"),".stl") || 
		StringTools::is_ending(cmd.get("scenery"),".STL")
	) {
		scenery = StereoLitographyIo::read(cmd.get("scenery"), 1.0);
	}else if(	
		StringTools::is_ending(cmd.get("scenery"),".xml") ||
		StringTools::is_ending(cmd.get("scenery"),".XML")
	) {
		WorldFactory fab;
		fab.load(cmd.get("scenery"));
		scenery = fab.world();
	}else{
		scenery = Frame::void_frame;
		cout << "Can only read stl or xml files.\n";
		return 0;
	}
	
	TracerSettings settings;

	if(!cmd.get("visual_config").empty()) {
		Xml::Document doc(cmd.get("visual_config"));
		Xml::Node node = doc.node();
		Xml::Node vc_node = node.child("visual");
		settings.visual = Xml::Configs::get_VisualConfig_from_node(vc_node);
	}

	FlyingCamera free(scenery, &settings);

	}catch(std::exception &error) {
		std::cerr << error.what(); 
	}
	return 0;
}