//=================================
// include guard
#ifndef __WriteCorsikaEventIoPhotonsRaw_H_INCLUDED__
#define __WriteCorsikaEventIoPhotonsRaw_H_INCLUDED__

#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include "Core/TracerException.h"

using std::string;
using std::stringstream;
using std::vector;
using std::array;
using std::istream;
using std::ifstream;

namespace EventIo {

    void write_raw_photons(const string &path, vector<array<float, 8>>);
} //namespace EventIo

#endif // __WriteCorsikaEventIoPhotonsRaw_H_INCLUDED__ 