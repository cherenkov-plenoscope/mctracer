// Copyright 2014 Sebastian A. Mueller
#ifndef SIGNAL_PROCESSING_SIMULATION_TRUTH_H_
#define SIGNAL_PROCESSING_SIMULATION_TRUTH_H_

#include "PipelinePhoton.h"
#include "PhotonStream.h"
#include "ElectricPulse.h"
#include "ExtractedPulse.h"
#include "pulse_extraction.h"
#include "PhotoElectricConverter.h"

namespace signal_processing {

const int DEFAULT_SIMULATION_TRUTH = -1;
const int NIGHT_SKY_BACKGROUND = -100;
const int PHOTO_ELECTRIC_CONVERTER_ACCIDENTAL = - 201;
const int PHOTO_ELECTRIC_CONVERTER_CROSSTALK = - 202;

}  // namespace signal_processing

#endif  // SIGNAL_PROCESSING_SIMULATION_TRUTH_H_
