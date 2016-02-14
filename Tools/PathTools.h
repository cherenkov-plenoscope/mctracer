//=================================
// include guard
#ifndef __PathTools_H_INCLUDE__
#define __PathTools_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies 
#include "StringTools.h"

namespace PathTools {
	
	struct FullPath {
		std::string path;
		std::string filename;
	};

	FullPath split_path_and_filename(std::string path);
}

#endif // __PathTools_H_INCLUDE__