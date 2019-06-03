// Copyright 2014 Sebastian A. Mueller
#include "merlict_portal_plenoscope/calibration/Calibrator.h"
#include <future>
#include <fstream>
#include <sstream>
#include <thread>
#include <iomanip>
#include <random>
#include <iostream>
#include "merlict/merlict.h"
#include "merlict_portal_plenoscope/night_sky_background/NightSkyBackground.h"
#include "merlict_multi_thread/vitaliy_vitsentiy_thread_pool.h"
namespace ml = merlict;


namespace plenoscope {
namespace calibration {

Calibrator::Calibrator(
    const Config _config,
    const PlenoscopeInScenery *_plenoscope,
    const ml::Frame* _world):
        config(_config),
        plenoscope(_plenoscope),
        world(_world),
        num_photons(config.photons_per_block*config.num_blocks),
        MAX_APERTURE_PLANE_RADIUS(
            night_sky_background::APERTURE_RADIUS_OVERHEAD*
            plenoscope->light_field_sensor_geometry.
                expected_imaging_system_max_aperture_radius()),
        MAX_INCIDENT_ANGLE(
            night_sky_background::FOV_RADIUS_OVERHEAD*
            plenoscope->light_field_sensor_geometry.max_FoV_radius()
        ) {}


ml::Photon create_photon(
    ml::Vec3 support_on_aperture,
    ml::Vec3 incident_direction
) {
    const ml::Ray back_running_ray(support_on_aperture, incident_direction);

    const ml::Vec3 creation_position = back_running_ray.position_at(
        DISTANCE_TO_APERTURE_PLANE);

    return ml::Photon(creation_position, incident_direction*(-1.0), WAVELENGTH);
}


CalibrationPhotonResult one_photon(
    int id,
    const uint64_t seed,
    const Calibrator &cal,
    const ml::random::ZenithDistancePicker &zenith_picker,
    const ml::random::UniformPicker &azimuth_picker)
{
    (void)id;
    ml::random::Mt19937 prng(seed);

    // create photon
    const ml::Vec3 support_on_aperture = prng.get_point_on_xy_disc_within_radius(
        cal.MAX_APERTURE_PLANE_RADIUS);

    const ml::Vec3 incident_direction = ml::random::draw_point_on_sphere(
        &prng,
        zenith_picker,
        azimuth_picker);

    ml::Photon ph = create_photon(support_on_aperture, incident_direction);

    // propagate photon
    ml::PropagationEnvironment env;
    env.root_frame = cal.world;
    env.prng = &prng;
    ml::Propagator(&ph, env);

    ml::sensor::FindSensorByFrame sensor_finder(
        ph.final_intersection().object(),
        &cal.plenoscope->light_field_channels->by_frame);

    if (sensor_finder.is_absorbed_by_known_sensor) {
        // remember photon
        CalibrationPhotonResult result;
        result.reached_sensor = true;
        result.lixel_id = sensor_finder.final_sensor->id;
        result.x_pos_on_principal_aperture = support_on_aperture.x;
        result.y_pos_on_principal_aperture = support_on_aperture.y;
        result.x_tilt_vs_optical_axis = incident_direction.x;
        result.y_tilt_vs_optical_axis = incident_direction.y;
        result.relative_time_of_flight = ph.time_of_flight();
        return result;
    } else {
        CalibrationPhotonResult result;
        result.reached_sensor = false;
        return result;
    }
}


void fill_another_block(
    const Calibrator &cal,
    LixelStatisticsFiller *lixel_statistics_filler,
    ml::random::Generator *prng)
{
    std::vector<CalibrationPhotonResult> photon_results(
        cal.config.photons_per_block);

    const ml::random::ZenithDistancePicker zenith_picker(
        0.0,
        cal.MAX_INCIDENT_ANGLE);
    const ml::random::UniformPicker azimuth_picker(
        0.0,
        2*M_PI);

    std::vector<uint64_t> prng_seeds(photon_results.size());
    for (uint64_t i = 0; i < photon_results.size(); i ++) {
        prng_seeds[i] = prng->create_seed();
    }

    uint64_t num_threads = std::thread::hardware_concurrency();
    merlict::ctpl::thread_pool pool(num_threads);
    std::vector<std::future<CalibrationPhotonResult>> futs(
        photon_results.size());
    for (uint64_t i = 0; i < futs.size(); ++i) {
        futs[i] = pool.push(
            one_photon,
            prng_seeds[i],
            cal,
            zenith_picker,
            azimuth_picker);
    }

    for (uint64_t i = 0; i < futs.size(); i ++) {
        photon_results[i] = futs[i].get();
    }

    lixel_statistics_filler->fill_in_block(photon_results);
}

void run_calibration(
    const Calibrator &cal,
    const std::string &path,
    ml::random::Generator *prng
) {
    LixelStatisticsFiller lixel_statistics_filler(
        cal.plenoscope->light_field_sensor_geometry.num_lixel(),
        cal.config.num_blocks,
        cal.config.photons_per_block);

    std::cout << "Plenoscope Calibrator: propagating ";
    std::cout << double(cal.num_photons)/1.0e6 << "e6 photons\n";

    for (uint64_t block = 0; block < cal.config.num_blocks; block++) {
        std::cout << block + 1 << " of " << cal.config.num_blocks << "\n";
        fill_another_block(
            cal,
            &lixel_statistics_filler,
            prng);
    }
    std::vector<LixelStatistic> lixel_statistics =
        lixel_statistics_filler.get_lixel_statistics();
    write(lixel_statistics, path);
}


void run_calibration_raw_output(
    const Calibrator &cal,
    const std::string &path,
    merlict::random::Generator *prng
) {
    const ml::random::ZenithDistancePicker zenith_picker(
        0.0,
        cal.MAX_INCIDENT_ANGLE);
    const ml::random::UniformPicker azimuth_picker(
        0.0,
        2*M_PI);

    std::ofstream fout;
    fout.open(path, std::ios::out | std::ios::binary);
    if (!fout.is_open()) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "Lightfield-Calibration: Unable to write to file: '";
        info << path << "'\n";
        throw std::runtime_error(info.str());
    }

    uint64_t photon_idx;
    for (photon_idx = 0; photon_idx < cal.num_photons; photon_idx++) {
        CalibrationPhotonResult res = one_photon(
            photon_idx,
            prng->create_seed(),
            cal,
            zenith_picker,
            azimuth_picker);

        fout.write((char*)&res, sizeof(CalibrationPhotonResult));
    }

    fout.close();
}

}  // namespace calibration
}  // namespace plenoscope
