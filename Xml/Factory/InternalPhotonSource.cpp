#include "InternalPhotonSource.h"
using StringTools::is_equal;

namespace Xml {
	namespace InternalPhotonSource {
//------------------------------------------------------------------------------
vector<Photon*>* node2photons(const Xml::Node &node) {
	// this is the lightsource node
	vector<Photon*>* photons;
	std::stringstream info;
	try{

		info << "try: point_source\n";
		photons = pointsource(node.child("point_source"));
    }catch(std::exception &error) {

        info << error.what();
        try{

       		info << "try: parallel_disc\n";
			photons = parallel_disc(node.child("parallel_disc"));
        }catch(std::exception &error) {

        	info << error.what();	
			info << __FILE__ << ", " << __LINE__ << "\n";
			info << "InternalPhotonSource from Xml\n";
			info << "Do not know any type of lightsource called ";
			info << "'" << node.name() << "'.\n";
			info << "Choose: point_source or parallel_disc\n";
			throw UnkownTypeOfLightSource(info.str());
        }
    }

	return photons;
}
//------------------------------------------------------------------------------
vector<Photon*>* pointsource(const Xml::Node &node) {
	
	vector<Photon*>* photons = Photons::Source::point_like_towards_z_opening_angle_num_photons(
		Deg2Rad(Xml::att2double(node,"opening_angle_in_deg")),
		int(Xml::att2double(node, "number_of_photons"))
	);
	transform(node, photons);
	return photons;
}
//------------------------------------------------------------------------------
vector<Photon*>* parallel_disc(const Xml::Node &node) {
	
	vector<Photon*>* photons = Photons::Source::parallel_towards_z_from_xy_disc(
		Xml::att2double(node,"disc_radius_in_m"),
		int(Xml::att2double(node, "number_of_photons"))
	);
	transform(node, photons);
	return photons;
}
//------------------------------------------------------------------------------
void transform(const Xml::Node &node, vector<Photon*>* photons) {
	
	Vector3D pos = Xml::att2Vector3D(node, "pos");
	Tuple3 rot_deg = Xml::att2Tuple3(node, "rot_in_deg");
	Rotation3D rot(Deg2Rad(rot_deg.x), Deg2Rad(rot_deg.y), Deg2Rad(rot_deg.z));

	HomoTrafo3D Trafo;
	Trafo.set_transformation(rot, pos);
	Photons::transform_all_photons_multi_thread(Trafo, photons);	
}
//------------------------------------------------------------------------------
	}//InternalPhotonSource
}//Xml