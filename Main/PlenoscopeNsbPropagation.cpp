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
    out << "  --output, -o      output path of plenoscope lightfields\n";
    out << "  --number, -n      number of events to be simulated\n";
    return out.str();   
}

int main(int argc, char* argv[]) {
    try{

    CommandLine::Parser cmd;
    cmd.define_key_val_by_key_short_desc("config", 'c' ,"config path steering the simulation and plenoscope");
    cmd.define_key_val_by_key_short_desc("output", 'o', "output path of plenoscope lightfields");
    cmd.define_key_val_by_key_short_desc("number", 'n', "number of events to be simulated");
    cmd.parse(argc, argv);

    if(!cmd.exist("config") || !cmd.exist("output")) {
        cout << help_text();
        return 0;
    }
    uint max_number_events = (uint)StringTools::to_int(cmd.get("number"));
    PathTools::Path config_path = PathTools::Path(cmd.get("config"));
    string working_directory = config_path.path;

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


    //--------------------------------------------------------------------------
    // propagate each event
    for(uint event_counter = 1; event_counter <= max_number_events; event_counter++){

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
        evt_header << "MMCS_Run_Header_________________________________________________\n";
        evt_header << "| run_number NA\n";
        evt_header << "| date_of_begin_run NA\n";
        evt_header << "| version_of_program NA\n";
        evt_header << "| number_of_observation_levels NA\n";
        evt_header << "| observation_levels NA \n";
        evt_header << "| slope_of_energy_spektrum NA\n";
        evt_header << "| energy_range NA NA GeV\n";
        evt_header << "| flag_for_EGS4_treatment_of_em_component NA\n";
        evt_header << "| flag_for_NKG_treatment_of_em_component NA\n";
        evt_header << "| kin_energy_cutoff_for_hadrons_in_GeV NA\n";
        evt_header << "| kin_energy_cutoff_for_muons_in_GeV NA\n";
        evt_header << "| kin_energy_cutoff_for_electrons_in_GeV NA\n";
        evt_header << "| energy_cutoff_for_photons_in_GeV NA\n";
        evt_header << "| phyiscal_constants NA\n";
        evt_header << "| X_displacement_of_inclined_observation_plane NA\n";
        evt_header << "| Y_displacement_of_inclined_observation_plane NA\n";
        evt_header << "| Z_displacement_of_inclined_observation_plane NA\n";
        evt_header << "| theta_angle_of_normal_vector_of_inclined_observation_plane NA\n";
        evt_header << "| phi_angle_of_normal_vector_of_inclined_observation_plane NA\n";
        evt_header << "| CKA NA \n";
        evt_header << "| CETA NA \n";
        evt_header << "| CSTRBA NA \n";
        evt_header << "| scatter_range_in_x_direction_for_Cherenkov NA\n";
        evt_header << "| scatter_range_in_y_direction_for_Cherenkov NA\n";
        evt_header << "| HLAY NA \n";
        evt_header << "| AATM NA \n";
        evt_header << "| BATM NA \n";
        evt_header << "| CATM NA \n";
        evt_header << "| NFLAIN NA\n";
        evt_header << "| NFLDIF NA\n";
        evt_header << "| NFLPI0_plus_100_times_NFLPIF NA\n";
        evt_header << "| NFLCHE_plus_100_times_NFRAGM NA\n";
        evt_header << "|_______________________________________________________________\n";
        evt_header << "Event_Header\n";
        evt_header << "core positions: \n";
        evt_header << " x.................... 0.0 cm\n";
        evt_header << " y.................... 0.0 cm\n";
        evt_header << "Telescope pointing\n";
        evt_header << " Az................... 0 deg\n";
        evt_header << " Zd................... 0 deg\n";
        evt_header << "Primary particle\n";
        evt_header << " Corsika ID........... " << event_counter << "\n";
        evt_header << " E.................... 0.0 GeV\n";
        evt_header << " start altitude....... 0 g/cm^2\n";
        evt_header << " first interaction z.. 0.0 m\n";
        evt_header << " n. obs. levels....... 0\n";
        evt_header << "\n";
        evt_header << "arrival_time[s]\tnumber_photons[1]\n";

        AsciiIo::write_table_to_file_with_header(
            dtacs, cmd.get("output") + std::to_string(event_counter) + ".txt",
            evt_header.str()
        );

        cout << "event " << event_counter << "\n";
    }

    }catch(std::exception &error) {

        std::cerr << error.what(); 
    }
    return 0;
}