// Copyright 2014 Sebastian A. Mueller
#ifndef PLENOSCOPE_CALIBRATION_CALIBRATOR_H_
#define PLENOSCOPE_CALIBRATION_CALIBRATOR_H_

#include <vector>
#include <string>
#include "plenoscope/PlenoscopeInScenery.h"
#include "plenoscope/calibration/Config.h"
#include "plenoscope/calibration/CalibrationPhotonResult.h"
#include "plenoscope/calibration/LixelStatisticsFiller.h"
#include "merlict/merlict.h"

namespace plenoscope {
namespace calibration {

struct Calibrator {
    const Config config;
    const PlenoscopeInScenery* plenoscope;
    const relleums::Frame* world;
    const unsigned int num_photons;
    const double MAX_APERTURE_PLANE_RADIUS;
    const double MAX_INCIDENT_ANGLE;

    Calibrator(
        const Config _calib_config,
        const PlenoscopeInScenery *plenoscope,
        const relleums::Frame* _scenery);
};

const double WAVELENGTH = 433e-9;
const double DISTANCE_TO_APERTURE_PLANE = 1e4;

void run_calibration(
    const Calibrator &cal,
    const std::string &path,
    relleums::random::Generator *prng);

}  // namespace calibration
}  // namespace plenoscope

#endif  // PLENOSCOPE_CALIBRATION_CALIBRATOR_H_
