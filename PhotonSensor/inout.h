//=================================
// include guard
#ifndef __PHOTONSENSORS_WRITER_H_INCLUDED__
#define __PHOTONSENSORS_WRITER_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "PhotonSensor/PhotonSensor.h"
#include <fstream>
//=================================

namespace PhotonSensor {

    void write_arrival_information_to_file(
        const vector<ArrivalInformation>* arrival_table,
        std::ofstream *file);

     vector<ArrivalInformation> read_arrival_information_from_file(
        std::ifstream *file,
        const uint number_of_arrivals);

} // PhotonSensor
//------------------------------------------------------------------------------
#endif // __PHOTONSENSORS_WRITER_H_INCLUDED__ 