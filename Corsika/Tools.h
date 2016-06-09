//=================================
// include guard
#ifndef __CORSIKATOOLS_H_INCLUDE__
#define __CORSIKATOOLS_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <array>
#include "Core/TracerException.h"
using std::string;
using std::vector;
using std::array;

//=================================
namespace Corsika {

	string get_run_header_print(const array<float, 273> &run_header);
	string get_event_header_print(const array<float, 273> &event_header);
	float str2float(const string word);
	string float2str(const float word_in_float);

	class Not4Chars : public TracerException {
		using TracerException::TracerException;
	};
}
#endif // __CORSIKATOOLS_H_INCLUDE__ 
