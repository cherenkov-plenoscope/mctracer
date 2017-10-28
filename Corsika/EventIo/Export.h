//=================================
// include guard
#ifndef __WriteCorsikaEventIoPhotonsRaw_H_INCLUDED__
#define __WriteCorsikaEventIoPhotonsRaw_H_INCLUDED__

#include <vector>
#include <array>
#include <iostream>
#include <fstream>

using std::string;
using std::stringstream;
using std::vector;
using std::array;
using std::istream;
using std::ifstream;

namespace EventIo {

    void write_raw_photons(
        const vector<array<float, 8>> &photons, 
        const string &path
    );
} //namespace EventIo

#endif // __WriteCorsikaEventIoPhotonsRaw_H_INCLUDED__ 