// Copyright 2014 Sebastian A. Mueller
#ifndef PLENOSCOPE_CALIBRATION_CALIBRATOR_H_
#define PLENOSCOPE_CALIBRATION_CALIBRATOR_H_

#include <vector>
#include <string>
#include "Plenoscope/PlenoscopeInScenery.h"
#include "Plenoscope/Calibration/Config.h"
#include "Plenoscope/Calibration/CalibrationPhotonResult.h"
#include "Plenoscope/Calibration/LixelStatisticsFiller.h"

namespace Plenoscope {
namespace Calibration {

class Calibrator {
    const Config config;
    const relleums::Frame* scenery;
    PlenoscopeInScenery* plenoscope;
    unsigned int number_of_photons;
    double max_principal_aperture_radius_to_trow_photons_on;
    double max_tilt_vs_optical_axis_to_throw_photons_in;
    double distance_to_travel_before_intersecting_principal_aperture;
    double callibration_photon_wavelenght;

    std::vector<CalibrationPhotonResult> photon_results;
    LixelStatisticsFiller lixel_statistics_filler;

 public:
    Calibrator(
        const Config _calib_config,
        PlenoscopeInScenery *plenoscope,
        const relleums::Frame* _scenery);
    void export_sub_pixel_statistics(const std::string path)const;
    void write_lixel_statistics(const std::string &path)const;
    std::string str()const;

 private:
    void set_up_photon_properties();
    void set_up_principal_aperture_range();
    void set_up_field_of_view_range();
    void set_up_plenoscope_environment();
    void run_calibration();
    void fill_calibration_block_to_table();
    relleums::Photon get_photon_given_pos_and_angle_on_principal_aperture(
        relleums::Vec3 pos_on_principal_aperture,
        relleums::Vec3 direction_on_principal_aperture)const;
};

}  // namespace Calibration
}  // namespace Plenoscope

#endif  // PLENOSCOPE_CALIBRATION_CALIBRATOR_H_
