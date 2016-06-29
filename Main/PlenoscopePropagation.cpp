#include "Tools/Tools.h"
#include "Tools/FileTools.h"
#include "CommandLine/CommandLine.h"
#include "Core/Photons.h"
#include "Corsika/EventIo/EventIo.h"
#include "Corsika/Tools.h"
#include "Corsika/EventIo/PhotonFactory.h"
#include "Core/Histogram1D.h"
#include "Tools/AsciiIo.h"
#include "Tools/FileTools.h"
#include "Tools/Tools.h"
#include "SignalProcessing/PipelinePhoton.h"
#include "Plenoscope/NightSkyBackground/Light.h"
#include "Plenoscope/EventFormats.h"
#include "Plenoscope/NightSkyBackground/Injector.h"
#include "SignalProcessing/PhotoElectricConverter.h"
#include "Xml/Xml.h"
#include "Xml/Factory/FunctionFromXml.h"
#include "Xml/Factory/SceneryFactory.h"
#include "Xml/Factory/TracerSettingsFab.h"
#include "SignalProcessing/SimpleTDCQDC.h"
#include "Tools/PathTools.h"
#include "Tools/Time.h"
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

string help_text() {
    std::stringstream out; 
    out << "  Plenoscope simulation\n";
    out << "  --scenery, -s     scenery with at least one plenoscope in it\n";
    out << "  --lixel_calib, -l light field cell calibration of the plenoscope\n";
    out << "  --config, -c      config path steering the simulation and the plenoscope\n";
    out << "  --input, -i       input path of CORSIKA run\n";
    out << "  --output, -o      output path of plenoscope lightfields\n";
    return out.str();   
}

int main(int argc, char* argv[]) {
    try{

    CommandLine::Parser cmd;
    cmd.define_key_val_by_key_short_desc("scenery", 's', "scenery with at least one plenoscope in it");
    cmd.define_key_val_by_key_short_desc("lixel_calib", 'l' ,"light field cell calibration of the plenoscope");
    cmd.define_key_val_by_key_short_desc("config", 'c' ,"config path steering the simulation and plenoscope");
    cmd.define_key_val_by_key_short_desc("output", 'o', "output path of plenoscope lightfields");
    cmd.define_key_val_by_key_short_desc("input", 'i', "input path of CORSIKA run");
    cmd.parse(argc, argv);

    if(!cmd.exist("config") || !cmd.exist("input") || 
       !cmd.exist("output") || !cmd.exist("scenery") || !cmd.exist("lixel_calib")) {
        cout << help_text();
        return 0;
    }
        
    //fs::create_directory(PathTools::join(cmd.get("output"),"sandbox"));

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
    // SET UP SCENERY
    Xml::SceneryFactory scenery_factory(cmd.get("scenery"));
    Frame scenery("root", Vec3::null, Rot3::null);
    scenery_factory.add_scenery_to_frame(&scenery);
    scenery.init_tree_based_on_mother_child_relations();
    if(scenery_factory.plenoscopes.size() == 0)
        throw TracerException("There is no plenoscope in the scenery");
    else if(scenery_factory.plenoscopes.size() > 1)
        throw TracerException("There is more then one plenoscope in the scenery");
    Plenoscope::PlenoscopeInScenery* pis = &scenery_factory.plenoscopes.at(0);

    PhotonSensors::Sensors* light_field_channels = pis->light_field_channels;

    //--------------------------------------------------------------------------
    // load light field calibration result
    vector<Plenoscope::Calibration::LixelStatistic> optics_calibration_result = 
        Plenoscope::Calibration::read(cmd.get("lixel_calib"));

    // assert number os sub_pixel matches simulated plenoscope
    if(light_field_channels->size() != optics_calibration_result.size()) {
        std::stringstream info;
        info << "The light field calibration results, read from file '";
        info << cmd.get("lixel_calib");
        info << "', do no not match the plenoscope simulated here.\n";
        info << "Expected sub pixel size: " << light_field_channels->size();
        info << ", but actual: " << optics_calibration_result.size();
        info << "\n";
        throw TracerException(info.str());
    }

    //--------------------------------------------------------------------------
    // INIT NIGHT SKY BACKGROUND
    Xml::Node nsb_node = config_node.child("night_sky_background_ligth");
    const Function::LinInterpol nsb_flux_vs_wavelength = 
        Xml::get_LinInterpol_from(nsb_node.child("function").child("linear_interpolation"));

    Plenoscope::NightSkyBackground::Light nsb(&pis->light_field_sensor_geometry, &nsb_flux_vs_wavelength);
    const double nsb_exposure_time = nsb_node.attribute2double("exposure_time");

    //--------------------------------------------------------------------------
    // SET UP PhotoElectricConverter
    Xml::Node pec = config_node.child("photo_electric_converter");
    const Function::LinInterpol pde_vs_wavelength = 
        Xml::get_LinInterpol_from(pec.child("function").child("linear_interpolation"));

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
        Time::StopWatch read_event("read_event");
        EventIo::Event event = corsika_run.next_event();
        read_event.stop();

        Time::StopWatch c2mct("corsika 2 mct photons");
        vector<Photon*> photons;
        uint photon_id = 0;
        for(const array<float, 8> &corsika_photon: event.photons) {
            
            EventIo::PhotonFactory cpf(corsika_photon, photon_id++, &prng);

            if(cpf.passed_atmosphere())
                photons.push_back(cpf.get_photon());
        }
        c2mct.stop();

        Time::StopWatch prop("propagate photons");
        Photons::propagate_photons_in_scenery_with_settings(
            &photons, &scenery, &settings, &prng
        );
        prop.stop();

        Time::StopWatch assign("assign photons to sensors");
        light_field_channels->clear_history();
        light_field_channels->assign_photons(&photons);
        Photons::delete_photons(&photons);

        vector<vector<PipelinePhoton>> photon_pipelines = 
            get_photon_pipelines(light_field_channels);
        assign.stop();

        Time::StopWatch nsbsw("night sky background");
        //-----------------------------
        // Night Sky Background photons
        Plenoscope::NightSkyBackground::inject_nsb_into_photon_pipeline(
            &photon_pipelines,
            nsb_exposure_time,
            &optics_calibration_result,
            &nsb,
            &prng
        );
        nsbsw.stop();

        Time::StopWatch pecs("photo electric conversion");
        //--------------------------
        // Photo Electric conversion
        vector<vector<double>> electric_pipelines;
        electric_pipelines.reserve(photon_pipelines.size());
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
        tacs.reserve(electric_pipelines.size());
        for(const vector<double> electric_pipe: electric_pipelines) {
            tacs.push_back(
                SimpleTdcQdc::get_arrival_time_and_count_given_arrival_moments_and_integration_time_window(
                    electric_pipe,
                    integration_time_window
                )
            );
        }
        pecs.stop();

        //-------------
        // export event
        Time::StopWatch write_event("write event");
        Plenoscope::save_event_to_file_epoch_2016May27(
            tacs,
            cmd.get("output") + std::to_string(event_counter) + ".txt"
        );
        write_event.stop();

        cout << "event " << event_counter << ", ";
        cout << "PRMPAR " << Corsika::EventHeader::particle_id(event.header.raw) << ", ";
        cout << "E " << Corsika::EventHeader::total_energy_in_GeV(event.header.raw) << " GeV\n";
        event_counter++;
    }

    }catch(std::exception &error) {

        std::cerr << error.what(); 
    }
    return 0;
}