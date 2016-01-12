#include "Propagation.h"
#include "Core/Photons.h"
#include "CorsikaIO/EventIo/EventIo.h"
#include "CorsikaIO/EventIo/PhotonFactory.h"
#include "Core/Histogram1D.h"
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
	std::cout << "coming soon :)\n";
	
	std::vector<double> wvl_bin_edges = numeric::linspace(250e-9, 750e-9, 25);

	Random::Mt19937 prng(Random::zero_seed);

	// load photons
	EventIo::EventIoFile corsika_run(input_path());

	// propagate each event
	uint event_counter = 0;
	while(corsika_run.has_still_events_left()) {

		event_counter++;

		// read next evenr
		EventIo::Event event = corsika_run.next_event();

		vector<Photon*> photons;
        uint id = 0;
        for(vector<float> corsika_photon: event.photons) {
            
            EventIo::PhotonFactory cpf(corsika_photon, id++, &prng);

            if(cpf.passed_atmosphere())
                photons.push_back(cpf.get_photon());
        }

        vector<double> wvl;
        for(Photon* ph: photons)
        	wvl.push_back(ph->get_wavelength());
        

        std::cout << "event " << event_counter << ", photons " << id << "\n";
        Histogram1D wvl_hist(wvl, wvl_bin_edges);
        std::cout << wvl_hist << "\n";

        // wipe out the cherenkov photons which have just been propagated
		Photons::delete_photons_and_history(&photons);

		
	} 
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