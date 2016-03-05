//=================================
// include guard
#ifndef __MMCSCORSIKATOOLS_H_INCLUDE__
#define __MMCSCORSIKATOOLS_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies  
#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <vector>
#include "Core/TracerException.h"

namespace MmcsCorsikaTools {
	
	float str2float_4byte_bin_map(const std::string word);
	
	std::string float2str_4byte_bin_map(const float word_in_float);
	
	std::string float_vec_2_str_using_delimiter(
		const std::vector<float> vec, 
		const std::string delimiter
	);
}

#endif // __MMCSCORSIKATOOLS_H_INCLUDE__