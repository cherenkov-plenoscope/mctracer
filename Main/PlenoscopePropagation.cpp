#include "Core/TracerException.h"
#include "Tools/Tools.h"
#include "Tools/FileTools.h"
#include "CommandLine/CommandLine.h"
#include "Core/Photons.h"
#include "CorsikaIO/EventIo/EventIo.h"
#include "CorsikaIO/EventIo/PhotonFactory.h"
#include "Core/Histogram1D.h"
#include "Plenoscope/Plenoscope.h"
#include "Tools/AsciiIo.h"
#include "Tools/FileTools.h"
#include "Tools/Tools.h"
#include "SignalProcessing/PipelinePhoton.h"
#include "Plenoscope/NightSkyBackgroundLight.h"
#include "Plenoscope/NightSkyBackgroundLightInjector.h"
#include "SignalProcessing/PhotoElectricConverter.h"
#include "Xml/Xml.h"
#include "Xml/Factory/TracerSettingsFab.h"
#include "SignalProcessing/SimpleTDCQDC.h"
#include "Tools/PathTools.h"

string help_text() {
    std::stringstream out; 
    out << "  Plenoscope simulation\n";
    out << "  --config, -c      config path steering the simulation and the plenoscope\n";
    out << "  --input, -i       input path of CORSIKA run\n";
    out << "  --output, -o      output path of plenoscope lightfields\n";
    return out.str();   
}

int main(int argc, char* argv[]) {
    try{

    CommandLine::Parser cmd;
    cmd.define_key_val_by_key_short_desc("config", 'c' ,"config path steering the simulation and plenoscope");
    cmd.define_key_val_by_key_short_desc("output", 'o', "output path of plenoscope lightfields");
    cmd.define_key_val_by_key_short_desc("input", 'i', "input path of CORSIKA run");
    cmd.parse(argc, argv);

    if(!cmd.exist("config") || !cmd.exist("input") || !cmd.exist("output")) {
        cout << help_text();
        return 0;
    }
        
    PathTools::Path config_path = PathTools::Path(cmd.get("config"));
    string working_directory = config_path.path;

    cout << "in     '" << cmd.get("input") << "'\n";
    cout << "out    '" << cmd.get("output") << "'\n";
    cout << "config '" << config_path.path << "'\n";

    //--------------------------------------------------------------------------
    //  111
    // 1111      set up simulation
    //   11
    //   11
    //   11
    //   11  
    // 111111 11
    //--------------------------------------------------------------------------
    Xml::Document doc(config_path.path);
    Xml::Node config_node = doc.node().child("propagation");

    //--------------------------------------------------------------------------
    // BASIC SETTINGS
    TracerSettings settings = Xml::Configs::get_TracerSettings_from_node(
        config_node.child("settings")
    );

    //--------------------------------------------------------------------------
    // INIT PRNG 
    Random::Mt19937 prng(settings.pseudo_random_number_seed);

    //--------------------------------------------------------------------------
    // SET UP TELESCOPE
    Plenoscope::Config telescope_config;
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
    telescope_config.lens_refraction = &Plenoscope::pmma_refraction;
    telescope_config.sub_pixel_on_pixel_diagonal = 13;
    telescope_config.object_distance_to_focus_on = 10.0e3;
    
    Plenoscope::Geometry telescope_geometry(telescope_config);
    Plenoscope::Factory fab(&telescope_geometry);
    
    Frame telescope("telescope", Vec3::null, Rot3::null);
    fab.add_telescope_to_frame(&telescope);
    PhotonSensors::Sensors* sensors = fab.get_sub_pixels();

    //--------------------------------------------------------------------------
    // load light field calibration result
    string optics_path = PathTools::join(
        config_path.dirname, 
        config_node.child("optics_calibration_result").attribute("path")
    );

    vector<vector<double>> optics_calibration_result = AsciiIo::gen_table_from_file(
        optics_path
    );
    // assert number os sub_pixel matches simulated telescope
    if(fab.get_sub_pixels()->size() != optics_calibration_result.size()) {
        std::stringstream info;
        info << "The light field calibration results, read from file '";
        info << optics_path;
        info << "', do no not match the telescope simulated here.\n";
        info << "Expected sub pixel size: " << fab.get_sub_pixels()->size();
        info << ", but actual: " << optics_calibration_result.size();
        info << "\n";
        throw TracerException(info.str());
    }

    //--------------------------------------------------------------------------
    // INIT NIGHT SKY BACKGROUND
    Xml::Node nsb_node = config_node.child("night_sky_background_ligth");
    const Function::LinInterpol nsb_flux_vs_wavelength(
        AsciiIo::gen_table_from_file(
            PathTools::join(
                config_path.dirname, 
                nsb_node.attribute("path_flux_vs_wavelength")
            )
        )
    );
    Plenoscope::NightSkyBackgroundLight nsb(&telescope_geometry, &nsb_flux_vs_wavelength);
    const double nsb_exposure_time = nsb_node.attribute2double("exposure_time");

    //--------------------------------------------------------------------------
    // SET UP PhotoElectricConverter
    Xml::Node pec = config_node.child("photo_electric_converter");

    const Function::LinInterpol pde_vs_wavelength(
        AsciiIo::gen_table_from_file(
            PathTools::join(
                config_path.dirname, 
                pec.attribute("path_pde_vs_wavelength")
            )
        )
    );

    PhotoElectricConverter::Config converter_config;
    converter_config.dark_rate = pec.attribute2double("dark_rate");
    converter_config.probability_for_second_puls = pec.attribute2double("probability_for_second_puls");
    converter_config.quantum_efficiency_vs_wavelength = &pde_vs_wavelength;

    PhotoElectricConverter::Converter sipm_converter(&converter_config);

    //--------------------------------------------------------------------------
    // SET UP PULSE EXTRACTOR
    Xml::Node pue = config_node.child("pulse_extractor");
    const double integration_time_window = pue.attribute2double("integration_time_window");

    //--------------------------------------------------------------------------
    //  2222
    // 22  22      run the simulation
    //    22
    //   22
    //  22
    // 22  
    // 222222 22
    //--------------------------------------------------------------------------
    // open cherenkov photon file
    EventIo::EventIoFile corsika_run(cmd.get("input"));
    vector<vector<double>> number_of_pulses_in_events;

    //--------------------------------------------------------------------------
    // propagate each event
    uint event_counter = 1;
    while(corsika_run.has_still_events_left()) {

        //------------------
        // Cherenkov photons
        EventIo::Event event = corsika_run.next_event();

        vector<Photon*> photons;
        uint photon_id = 0;
        for(array<float, 8> corsika_photon: event.photons) {
            
            EventIo::PhotonFactory cpf(corsika_photon, photon_id++, &prng);

            if(cpf.passed_atmosphere())
                photons.push_back(cpf.get_photon());
        }

        Photons::propagate_photons_in_world_with_settings(
            &photons, &telescope, &settings, &prng
        );

        sensors->clear_history();
        sensors->assign_photons(&photons);
        Photons::delete_photons(&photons);

        vector<vector<PipelinePhoton>> photon_pipelines = 
            get_photon_pipelines(sensors);

        //-----------------------------
        // Night Sky Background photons
        Plenoscope::inject_nsb_into_photon_pipeline(
            &photon_pipelines,
            nsb_exposure_time,
            &optics_calibration_result,
            &nsb,
            &prng
        );
        

        //--------------------------
        // Photo Electric conversion
        vector<vector<double>> electric_pipelines;
        for(vector<PipelinePhoton> ph_pipe: photon_pipelines) {

            electric_pipelines.push_back(
                sipm_converter.get_pulse_pipeline_for_photon_pipeline(
                    ph_pipe,
                    nsb_exposure_time,
                    &prng
                )
            );
        }

        //-------------------------
        // Pulse extraction Tdc Qdc
        vector<SimpleTdcQdc::TimeAndCount> tacs;
        for(const vector<double> electric_pipe: electric_pipelines) {
            tacs.push_back(
                SimpleTdcQdc::get_arrival_time_and_count_given_arrival_moments_and_integration_time_window(
                    electric_pipe,
                    integration_time_window
                )
            );
        }

        //-------------
        // export event
        vector<vector<double>> dtacs;
        for(SimpleTdcQdc::TimeAndCount tac: tacs) {
            vector<double> dtac = {tac.time, double(tac.count)};
            dtacs.push_back(dtac);
        }

        stringstream evt_header;
        evt_header << corsika_run.run_header.mmcs_runheader.get_print();
        evt_header << event.header.mmcs_event_header.get_print();
        evt_header << "\n";
        evt_header << "arrival_time[s]\tnumber_photons[1]\n";

        AsciiIo::write_table_to_file_with_header(
            dtacs, cmd.get("output") + std::to_string(event_counter) + ".txt",
            evt_header.str()
        );

        cout << "event " << event_counter << ", ";
        cout << "PRMPAR " << event.header.mmcs_event_header.particle_id << ", ";
        cout << "E " << event.header.mmcs_event_header.total_energy_in_GeV << " GeV\n";
        event_counter++;
    }

    }catch(std::exception &error) {

        std::cerr << error.what(); 
    }
    return 0;
}