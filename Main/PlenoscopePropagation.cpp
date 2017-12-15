#include "DocOpt/docopt.h"
#include "Tools/Tools.h"
#include "Tools/FileTools.h"
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
#include "Plenoscope/EventHeader.h"
#include "Plenoscope/SimulationTruthHeader.h"
#include "Plenoscope/NightSkyBackground/Injector.h"
#include "SignalProcessing/PhotoElectricConverter.h"
#include "Xml/Xml.h"
#include "Xml/Factory/FunctionFromXml.h"
#include "Xml/Factory/SceneryFactory.h"
#include "Xml/Factory/PropagationConfigFab.h"
#include "SignalProcessing/SimpleTDCQDC.h"
#include "SignalProcessing/ElectricPulse.h"
#include "SignalProcessing/PhotonStream.h"
#include "Tools/PathTools.h"
#include "Tools/HeaderBlock.h"
#include "Tools/Time.h"
#include "Scenery/Scenery.h"
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

using std::string;
using std::vector;
using std::array;
using std::cout;

static const char USAGE[] =
R"(Plenoscope air showher propagation

    Usage:
      mctPlenoscopePropagation -l=LIXEL_STATISTICS_PATH -c=CONFIG_PATH -i=CORSIKA_PATH -o=OUTPUT_PATH [-r=SEED] [--all_truth]
      mctPlenoscopePropagation (-h | --help)
      mctPlenoscopePropagation --version

    Options:
      -l --lixel=LIXEL_PATH     Light field calibration directory of the plenoscope.
      -c --config=CONFIG_PATH   Config path to xml file steering the simulation.
      -i --input=CORSIKA_PATH   CORSIKA run path.
      -o --output=OUTPUT_PATH   Output path.
      -r --random_seed=SEED     Seed for pseudo random number generator.
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

    PathTools::Path config_path = PathTools::Path(args.find("--config")->second.asString());
    PathTools::Path out_path = PathTools::Path(args.find("--output")->second.asString());
    PathTools::Path lixel_calib_path = PathTools::Path(args.find("--lixel")->second.asString());
    PathTools::Path input_path = PathTools::Path(args.find("--input")->second.asString());
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
    PropagationConfig settings = Xml::Configs::get_PropagationConfig_from_node(
        config_node.child("settings")
    );

    //--------------------------------------------------------------------------
    // INIT PRNG 
    Random::Mt19937 prng;
    if(args.find("--random_seed")->second)
        prng.set_seed(args.find("--random_seed")->second.asLong());

    //--------------------------------------------------------------------------
    // SET UP SCENERY
    Xml::SceneryFactory scenery_factory(scenery_path.path);

    Scenery scenery;

    scenery_factory.append_to_frame_in_scenery(&scenery.root, &scenery);
    scenery.root.init_tree_based_on_mother_child_relations();
    if(scenery_factory.plenoscopes.size() == 0)
        throw std::invalid_argument("There is no plenoscope in the scenery");
    else if(scenery_factory.plenoscopes.size() > 1)
        throw std::invalid_argument("There is more then one plenoscope in the scenery");
    Plenoscope::PlenoscopeInScenery* pis = &scenery_factory.plenoscopes.at(0);

    PhotonSensor::Sensors* light_field_channels = pis->light_field_channels;

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
        info << "Expected number of light field channels: " << light_field_channels->size();
        info << ", but actual: " << optics_calibration_result.size();
        info << "\n";
        throw std::invalid_argument(info.str());
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

    SignalProcessing::PhotoElectricConverter::Config converter_config;
    converter_config.dark_rate = pec.attribute2double("dark_rate");
    converter_config.probability_for_second_puls = pec.attribute2double("probability_for_second_puls");
    converter_config.quantum_efficiency_vs_wavelength = &pde_vs_wavelength;

    SignalProcessing::PhotoElectricConverter::Converter sipm_converter(
        &converter_config);

    //--------------------------------------------------------------------------
    // SET SINGLE PULSE OUTPUT
    Xml::Node spe = config_node.child("photon_stream");
    const double slice_duration = spe.attribute2double("slice_duration");

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

    //--------------------------------------------------------------------------
    // propagate each event
    unsigned int event_counter = 1;
    while(corsika_run.has_still_events_left()) {

        //------------------
        // Cherenkov photons
        EventIo::Event event = corsika_run.next_event();

        vector<Photon> photons;
        unsigned int photon_id = 0;

        for(const array<float, 8> &corsika_photon: event.photons) {
            EventIo::PhotonFactory cpf(corsika_photon, photon_id++, &prng);
            if(cpf.passed_atmosphere())
                photons.push_back(cpf.get_photon());
        }

        Photons::propagate_photons_in_scenery_with_settings(
            &photons, &scenery.root, &settings, &prng
        );

        light_field_channels->clear_history();
        light_field_channels->assign_photons(&photons);

        vector<vector<SignalProcessing::PipelinePhoton>> photon_pipelines = 
            SignalProcessing::get_photon_pipelines(light_field_channels);

        //-----------------------------
        // Night Sky Background photons
        Plenoscope::NightSkyBackground::inject_nsb_into_photon_pipeline(
            &photon_pipelines,
            nsb_exposure_time,
            &optics_calibration_result,
            &nsb,
            &prng
        );

        //--------------------------
        // Photo Electric conversion
        vector<vector<SignalProcessing::ElectricPulse>> electric_pipelines;
        electric_pipelines.reserve(photon_pipelines.size());
        for(vector<SignalProcessing::PipelinePhoton> ph_pipe: photon_pipelines) {

            electric_pipelines.push_back(
                sipm_converter.get_pulse_pipeline_for_photon_pipeline(
                    ph_pipe,
                    nsb_exposure_time,
                    &prng
                )
            );
        }

        //-------------
        // export event
        PathTools::Path event_output_path = PathTools::join(out_path.path, std::to_string(event_counter));
        fs::create_directory(event_output_path.path);

        SignalProcessing::PhotonStream::write(
            electric_pipelines,
            slice_duration,
            PathTools::join(event_output_path.path, "raw_light_field_sensor_response.phs")
        );

        Plenoscope::EventHeader event_header;
        event_header.set_event_type(Plenoscope::EventTypes::SIMULATION);
        event_header.set_trigger_type(Plenoscope::TriggerType::EXTERNAL_TRIGGER_BASED_ON_AIR_SHOWER_SIMULATION_TRUTH);
        event_header.set_plenoscope_geometry(pis->light_field_sensor_geometry.config);
        HeaderBlock::write(
            event_header.raw, 
            PathTools::join(event_output_path.path, "event_header.bin")
        );

        //-------------
        // export Simulation Truth
        PathTools::Path event_mc_truth_path = PathTools::join(event_output_path.path, "simulation_truth");
        fs::create_directory(event_mc_truth_path.path);
        HeaderBlock::write(corsika_run.header.raw, PathTools::join(event_mc_truth_path.path, "corsika_run_header.bin"));
        HeaderBlock::write(event.header.raw, PathTools::join(event_mc_truth_path.path, "corsika_event_header.bin"));
        
        Plenoscope::SimulationTruthHeader sim_truth_header;
        sim_truth_header.set_random_number_seed_of_run(prng.seed());
        HeaderBlock::write(
            sim_truth_header.raw, 
            PathTools::join(event_mc_truth_path.path, "mctracer_event_header.bin")
        );   

        if(export_all_simulation_truth) {
            SignalProcessing::PhotonStream::write_simulation_truth(
                electric_pipelines,
                PathTools::join(event_mc_truth_path.path, "detector_pulse_origins.bin")
            );

            EventIo::write_raw_photons(
                event.photons,
                PathTools::join(event_mc_truth_path.path, "air_shower_photon_bunches.bin")
            );
        }

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