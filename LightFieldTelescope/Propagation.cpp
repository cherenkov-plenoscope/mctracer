#include "Propagation.h"
#include "Core/Photons.h"
#include "CorsikaIO/EventIo/EventIo.h"
#include "CorsikaIO/EventIo/PhotonFactory.h"
#include "Core/Histogram1D.h"
#include "LightFieldTelescope/LightFieldTelescope.h"
#include "Tools/AsciiIo.h"
#include "Tools/FileTools.h"
#include "Tools/Tools.h"
//experimental
#include "PipelinePhoton.h"
#include "LightFieldTelescope/NightSkyBackgroundLightInjector.h"
#include "PhotoElectricConverter.h"
#include "PreTrigger.h"

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

	comand_line_parser.add<std::string>(
		nsbK, 'n', "night sky background flux vs wavelength", false, ""
	);

	comand_line_parser.add<std::string>(
		pdeK, 'q', "SIPM quantum efficiency vs wavelength path", false, ""
	);

	comand_line_parser.add<std::string>(
		sipm_pulseK, 's', "SIPM pulse template path", false, ""
	);
}
//------------------------------------------------------------------------------
void Propagation::execute() {

	std::cout << "out  '" << output_path() << "'\n";
	std::cout << "in   '" << input_path() << "'\n";
	std::cout << "conf '" << config_path() << "'\n";
	std::cout << "nsb  '" << nsb_vs_wavelength_path() << "'\n";
	std::cout << "pde  '" << pde_vs_wavelength_path() << "'\n";
	std::cout << "sipm '" << sipm_pulse_template_path() << "'\n";

	//--------------------------------------------------------------------------
	// settings
	TracerSettings settings;	
	settings.SetMultiThread(false);
	settings.store_only_last_intersection(true);

	//--------------------------------------------------------------------------
    // SET UP TELESCOPE
    LightFieldTelescope::Config telescope_config;
    telescope_config.reflector.focal_length = 75.0;
    telescope_config.reflector.DaviesCotton_over_parabolic_mixing_factor = 0.0;
    telescope_config.reflector.max_outer_aperture_radius = 25.0;
    telescope_config.reflector.min_inner_aperture_radius = 0.5;
    telescope_config.reflector.facet_inner_hex_radius = 0.6;
    telescope_config.reflector.gap_between_facets = 0.01;
    telescope_config.reflector.reflectivity = &SegmentedReflector::perfect_reflectivity;
    telescope_config.max_FoV_diameter = Deg2Rad(6.5);
    telescope_config.pixel_FoV_hex_flat2flat = Deg2Rad(0.0667);
    telescope_config.housing_overhead = 1.2;
    telescope_config.lens_refraction = &LightFieldTelescope::pmma_refraction;
    telescope_config.sub_pixel_on_pixel_diagonal = 7;
    telescope_config.object_distance_to_focus_on = 10.0e3;
	
    LightFieldTelescope::Geometry telescope_geometry(telescope_config);

    LightFieldTelescope::Factory fab(&telescope_geometry);
    Frame telescope("telescope", Vector3D::null, Rotation3D::null);
    fab.add_telescope_to_frame(&telescope);
    PhotonSensors::Sensors* sensors = fab.get_sub_pixels();

    //--------------------------------------------------------------------------
    // load light field calibration result
    std::vector<std::vector<double>> light_field_calibration_result = 
    	AsciiIo::gen_table_from_file(config_path());
    	
    	// assert number os sub_pixel matches simulated telescope
    	if(fab.get_sub_pixels()->size() != light_field_calibration_result.size()) {
    		std::stringstream info;
    		info << "The light field calibration results, read from file '";
    		info << config_path();
    		info << "', do no not match the telescope simulated here.\n";
    		info << "Expected sub pixel size: " << fab.get_sub_pixels()->size();
    		info << ", but actual: " << light_field_calibration_result.size();
    		info << "\n";
    		throw TracerException(info.str());
    	}

	//--------------------------------------------------------------------------
    // INIT PRNG 
	Random::Mt19937 prng(Random::zero_seed);

	//--------------------------------------------------------------------------
	// INIT NIGHT SKY BACKGROUND
	const Function::LinInterpol nsb_flux_vs_wavelength(
		AsciiIo::gen_table_from_file(nsb_vs_wavelength_path())
	);

	NightSkyBackgroundLight nsb(&telescope_geometry, &nsb_flux_vs_wavelength);

	//--------------------------------------------------------------------------
	// SET UP PhotoElectricConverter
	const Function::LinInterpol pde_vs_wavelength(
		AsciiIo::gen_table_from_file(pde_vs_wavelength_path())
	);
	PhotoElectricConverter::Config converter_config;
	converter_config.dark_rate = 1e6;
	converter_config.probability_for_second_puls = 0.1;
	converter_config.quantum_efficiency_vs_wavelength = &pde_vs_wavelength;

	PhotoElectricConverter::Converter sipm_converter(&converter_config);

	//--------------------------------------------------------------------------
	// EXPOSURE TIME
	const double exposure_time = 300e-9;

	//--------------------------------------------------------------------------
	// PRE TRIGGER
	PreTrigger::Config pre_trigger_config;
	pre_trigger_config.time_window = 50e-9;
	pre_trigger_config.threshold = 5;

	//--------------------------------------------------------------------------
	// Electric time line sampler
	PhotoElectricConverter::SamplerConfig sampler_config;
	sampler_config.exposure_time = exposure_time;
	sampler_config.sampling_frequency = 2e9;
	sampler_config.std_dev_noise = 0.2;

	PhotoElectricConverter::Sampler time_line_sampler(&sampler_config);

    //--------------------------------------------------------------------------
    // INIT ELECTRIC SIPM PULSE TEMPLATE
    const Function::LinInterpol sipm_pulse_template(
		AsciiIo::gen_table_from_file(sipm_pulse_template_path())
	);

	Function::DiscretSampling::Config sipm_pulse_discrete_sampling_config;
	sipm_pulse_discrete_sampling_config.slice_width = 1.0/sampler_config.sampling_frequency;
	sipm_pulse_discrete_sampling_config.samples_per_slice = 100;

	Function::DiscretSampling::LookUpTable sipm_pulse_discrete_samples(
		&sipm_pulse_template,
		sipm_pulse_discrete_sampling_config
	);

	//--------------------------------------------------------------------------
	// open cherenkov photon file
	EventIo::EventIoFile corsika_run(input_path());

	//--------------------------------------------------------------------------
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

		std::vector<std::vector<PipelinePhoton>> photon_pipelines = 
			get_photon_pipelines(sensors);

		LightFieldTelescope::inject_nsb_into_photon_pipeline(
			&photon_pipelines,
			exposure_time,
			&light_field_calibration_result,
			&nsb,
			&prng
		);

		std::vector<std::vector<double>> electric_pipelines;

		for(std::vector<PipelinePhoton> ph_pipe: photon_pipelines) {

			electric_pipelines.push_back(
				sipm_converter.get_pulse_pipeline_for_photon_pipeline( //bug
					ph_pipe,
					exposure_time,
					&prng
				)
			);
		}

		std::vector<uint> might_trigger;
		std::vector<uint> trigger;

		std::vector<std::vector<double>> time_lines;
		for(uint i=0; i<electric_pipelines.size(); i++) {
			
			if(PreTrigger::might_trigger(&electric_pipelines[i], pre_trigger_config)){
				might_trigger.push_back(i);
				std::vector<double> time_line;
				
				time_line = time_line_sampler.time_line(
					&electric_pipelines[i],
					&sipm_pulse_discrete_samples,
					&prng
				);

				time_lines.push_back(time_line);

				for(double s: time_line) {
					if(s > pre_trigger_config.threshold)
						trigger.push_back(i);
				}

				//std::cout << time_line_sampler.get_number_of_samples() << ", " << time_line.size() << "\n";
			}
		}

		AsciiIo::write_table_to_file(time_lines, output_path()+std::to_string(event_counter));
		//std::cout << get_print(electric_pipelines);

		std::cout << "mt " << might_trigger.size() << " of " << electric_pipelines.size() <<
		", " << double(might_trigger.size())/double(electric_pipelines.size())<< ", actual t " << trigger.size()<< "\n";

        std::cout << "event " << event_counter << ", E ";
        std::cout << event.header.mmcs_event_header.total_energy_in_GeV;
        std::cout << " GeV, photons " << photons.size() << "\n";

		Photons::delete_photons_and_history(&photons);

		event_counter++;
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
std::string Propagation::nsb_vs_wavelength_path()const {
	return comand_line_parser.get<std::string>(nsbK);
}
//------------------------------------------------------------------------------
std::string Propagation::pde_vs_wavelength_path()const {
	return comand_line_parser.get<std::string>(pdeK);
}
//------------------------------------------------------------------------------
std::string Propagation::sipm_pulse_template_path()const {
	return comand_line_parser.get<std::string>(sipm_pulseK);
}
}// LightFieldTelescope