#include "Propagation.h"
#include "Core/Photons.h"
#include "CorsikaIO/EventIo/EventIo.h"
#include "CorsikaIO/EventIo/PhotonFactory.h"
#include "Core/Histogram1D.h"
#include "LightFieldTelescope/LightFieldTelescope.h"
#include "Tools/AsciiIo.h"

namespace LightFieldTelescope {
//------------------------------------------------------------------------------
Propagation::Propagation(int argc, char** argv) {

	define_comand_line_keys();
	comand_line_parser.parse_check(argc, argv);
	execute();
}
//------------------------------------------------------------------------------
void Propagation::define_comand_line_keys() {

	comand_line_parser.add<std::string>(
		configK, 'c' ,"configuration file controling the simulation" , false, ""
	);

	comand_line_parser.add<std::string>(
		outputK, 'o', "output path", false, ""
	);

	comand_line_parser.add<std::string>(
		inputK, 'i', "input path for CORSIKA Cherenkov photons", false, ""
	);
}
//------------------------------------------------------------------------------
void Propagation::execute() {

	std::cout << "out  '" << output_path() << "'\n";
	std::cout << "in   '" << input_path() << "'\n";
	std::cout << "conf '" << config_path() << "'\n";
	
	// check output write access

	// settings
	TracerSettings settings;	
	settings.SetMultiThread(false);
	settings.store_only_last_intersection(true);

    // SET UP TELESCOPE
    LightFieldTelescope::Config telescope_config;
    telescope_config.reflector.focal_length = 75.0;
    telescope_config.reflector.DaviesCotton_over_parabolic_mixing_factor = 1.0;
    telescope_config.reflector.max_outer_aperture_radius = 25.0;
    telescope_config.reflector.min_inner_aperture_radius = 0.5;
    telescope_config.reflector.facet_inner_hex_radius = 0.6;
    telescope_config.reflector.gap_between_facets = 0.01;
    telescope_config.reflector.reflectivity = &SegmentedReflector::perfect_reflectivity;
    telescope_config.max_FoV_diameter = Deg2Rad(6.5);
    telescope_config.pixel_FoV_hex_flat2flat = Deg2Rad(0.1);
    telescope_config.housing_overhead = 1.2;
    telescope_config.lens_refraction = &LightFieldTelescope::pmma_refraction;
    telescope_config.sub_pixel_on_pixel_diagonal = 11;
	
    // INIT GEOMETRY
    LightFieldTelescope::Geometry telescope_geometry(telescope_config);

    // CREATE TELESCOPE
    LightFieldTelescope::Factory fab(&telescope_geometry);
    Frame telescope("telescope", Vector3D::null, Rotation3D::null);
    fab.add_telescope_to_frame(&telescope);
    PhotonSensors::Sensors* sensors = fab.get_sub_pixels();

	Random::Mt19937 prng(Random::zero_seed);

	// load photons
	EventIo::EventIoFile corsika_run(input_path());

	std::vector<std::vector<double>> photon_counts;

	// propagate each event
	uint event_counter = 0;
	while(corsika_run.has_still_events_left()) {

		EventIo::Event event = corsika_run.next_event();

		vector<Photon*> photons;
        uint id = 0;
        for(vector<float> corsika_photon: event.photons) {
            
            EventIo::PhotonFactory cpf(corsika_photon, id++, &prng);

            if(cpf.passed_atmosphere())
                photons.push_back(cpf.get_photon());
        }

		Photons::propagate_photons_in_world_with_settings(
			&photons, &telescope, &settings
		);

		sensors->clear_history();
		sensors->assign_photons(&photons);

		std::vector<double> photon_count(sensors->size());
		for(uint i=0; i<sensors->size(); i++)
			photon_count[i] = sensors->by_occurence[i]->get_number_of_photons();
		photon_counts.push_back(photon_count);

        std::cout << "event " << event_counter << ", E ";
        std::cout << event.header.mmcs_event_header.total_energy_in_GeV;
        std::cout << " GeV, photons " << photons.size() << "\n";

		Photons::delete_photons_and_history(&photons);

		event_counter++;
	}

	AsciiIo::write_table_to_file(photon_counts, output_path());
}
//------------------------------------------------------------------------------
std::string Propagation::output_path()const {
	return comand_line_parser.get<std::string>(outputK);
}
//------------------------------------------------------------------------------
std::string Propagation::input_path()const {
	return comand_line_parser.get<std::string>(inputK);
}
//------------------------------------------------------------------------------
std::string Propagation::config_path()const {
	return comand_line_parser.get<std::string>(configK);
}
//------------------------------------------------------------------------------
}// LightFieldTelescope