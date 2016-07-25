#include "DocOpt/docopt.h"
#include "Tools/Tools.h"
#include "Tools/FileTools.h"
#include "CommandLine/CommandLine.h"
#include "Core/Photons.h"
#include "Corsika/EventIo/EventIo.h"
#include "Corsika/EventIo/Export.h"
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
#include "SignalProcessing/ElectricPulse.h"
#include "Tools/PathTools.h"
#include "Tools/HeaderBlock.h"
#include "Tools/Time.h"
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

static const char USAGE[] =
R"(Plenoscope air showher propagation

    Usage:
      mctPlenoscopePropagation -l <LIXEL_STATISTICS_PATH> -c <CONFIG_PATH> -i <CORSIKA_PATH> -o <OUTPUT_PATH> [--all_truth]
      mctPlenoscopePropagation (-h | --help)
      mctPlenoscopePropagation --version

    Options:
      -l --lixel_statistics     light field calibration directory of the plenoscope.
      -c --config               Config path to xml file steering the simulation.
      -i --input                CORSIKA run path.
      -o --output               Output path.
      --all_truth               Write all simulation truth avaiable into the output.
      -h --help                 Show this screen.
      --version                 Show version.
      
)";

int main(int argc, char* argv[]) {
    try {

    std::map<std::string, docopt::value> args = docopt::docopt(
        USAGE,
        { argv + 1, argv + argc },
        true,        // show help if requested
        "mct 0.0"
    );  // version string

    PathTools::Path config_path = PathTools::Path(args.find("<CONFIG_PATH>")->second.asString());
    PathTools::Path out_path = PathTools::Path(args.find("<OUTPUT_PATH>")->second.asString());
    PathTools::Path lixel_calib_path = PathTools::Path(args.find("<LIXEL_STATISTICS_PATH>")->second.asString());
    PathTools::Path input_path = PathTools::Path(args.find("<CORSIKA_PATH>")->second.asString());
    const bool export_all_simulation_truth = args.find("--all_truth")->second.asBool();

    // 1) create output directory
    fs::create_directory(out_path.path);
    // 2) copy input into output directory
    PathTools::Path input_copy_path = PathTools::join(out_path.path, "input");
    fs::create_directory(input_copy_path.path);
    fs::copy(config_path.path, PathTools::join(input_copy_path.path, "propagation_config.xml"));
    fs::create_hard_link(input_path.path,  PathTools::join(input_copy_path.path, input_path.basename));
    fs::copy(lixel_calib_path.path, PathTools::join(input_copy_path.path, "plenoscope"), fs::copy_options::recursive);            

    config_path = PathTools::join(input_copy_path.path, "propagation_config.xml");
    lixel_calib_path = PathTools::join(PathTools::join(input_copy_path.path, "plenoscope"),"lixel_statistics.bin");
    input_path = PathTools::join(input_copy_path.path, input_path.basename);
    PathTools::Path scenery_path =  PathTools::join(PathTools::join(input_copy_path.path, "plenoscope"),"input/scenery/scenery.xml");

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
    Xml::SceneryFactory scenery_factory(scenery_path.path);
    Frame scenery;
    scenery.set_name_pos_rot("root", Vec3::null, Rot3::null);
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
        Plenoscope::Calibration::read(lixel_calib_path.path);

    // assert number os sub_pixel matches simulated plenoscope
    if(light_field_channels->size() != optics_calibration_result.size()) {
        std::stringstream info;
        info << "The light field calibration results, read from file '";
        info << lixel_calib_path.path;
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
    EventIo::Run corsika_run(input_path.path);
    vector<vector<double>> number_of_pulses_in_events;

    //--------------------------------------------------------------------------
    // propagate each event
    uint event_counter = 1;
    while(corsika_run.has_still_events_left()) {

        //------------------
        // Cherenkov photons
        //Time::StopWatch read_event("read_event");
        EventIo::Event event = corsika_run.next_event();
        //read_event.stop();

        //Time::StopWatch c2mct("corsika 2 mct photons");
        vector<Photon*> photons;
        uint photon_id = 0;

        for(const array<float, 8> &corsika_photon: event.photons) {
            EventIo::PhotonFactory cpf(corsika_photon, photon_id++, &prng);
            if(cpf.passed_atmosphere())
                photons.push_back(cpf.get_photon());
        }
        //c2mct.stop();

        //Time::StopWatch prop("propagate photons");
        Photons::propagate_photons_in_scenery_with_settings(
            &photons, &scenery, &settings, &prng
        );
        //prop.stop();

        //Time::StopWatch assign("assign photons to sensors");
        light_field_channels->clear_history();
        light_field_channels->assign_photons(&photons);
        Photons::delete_photons(&photons);

        vector<vector<PipelinePhoton>> photon_pipelines = 
            get_photon_pipelines(light_field_channels);
        //assign.stop();

        //Time::StopWatch nsbsw("night sky background");
        //-----------------------------
        // Night Sky Background photons
        Plenoscope::NightSkyBackground::inject_nsb_into_photon_pipeline(
            &photon_pipelines,
            nsb_exposure_time,
            &optics_calibration_result,
            &nsb,
            &prng
        );
        //nsbsw.stop();

        //Time::StopWatch pecs("photo electric conversion");
        //--------------------------
        // Photo Electric conversion
        vector<vector<ElectricPulse>> electric_pipelines;
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
        for(const vector<ElectricPulse> electric_pipe: electric_pipelines) {
            tacs.push_back(
                SimpleTdcQdc::get_arrival_time_and_count_given_arrival_moments_and_integration_time_window(
                    electric_pipe,
                    integration_time_window
                )
            );
        }
        //pecs.stop();

        //-------------
        // export event
        //Time::StopWatch write_event("write event");
        PathTools::Path event_output_path = PathTools::join(out_path.path, std::to_string(event_counter));
        fs::create_directory(event_output_path.path);

        Plenoscope::save_event_to_file_epoch_2016May27(
            tacs,
            PathTools::join(event_output_path.path, "raw_light_field_sensor_response.bin")
        );

        HeaderBlock::write(
            pis->light_field_sensor_geometry.config.get_sensor_plane2imaging_system_header(), 
            PathTools::join(event_output_path.path, "sensor_plane2imaging_system.bin")
        );

        //-------------
        // export Simulation Truth
        PathTools::Path event_mc_truth_path = PathTools::join(event_output_path.path, "simulation_truth");
        fs::create_directory(event_mc_truth_path.path);
        HeaderBlock::write(corsika_run.header.raw, PathTools::join(event_mc_truth_path.path, "corsika_run_header.bin"));
        HeaderBlock::write(event.header.raw, PathTools::join(event_mc_truth_path.path, "corsika_event_header.bin"));
        
        if(export_all_simulation_truth) {
            SimpleTdcQdc::write_intensity_simulation_truth(
                tacs,
                PathTools::join(event_mc_truth_path.path, "intensity_truth.txt")
            );

            EventIo::write_raw_photons(
                event.photons,
                PathTools::join(event_mc_truth_path.path, "air_shower_photons.bin")
            );
        }
        //write_event.stop();

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