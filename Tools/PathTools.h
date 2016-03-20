//=================================
// include guard
#ifndef __PathTools_H_INCLUDE__
#define __PathTools_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies 
#include "StringTools.h"
using std::string;

namespace PathTools {
	
	struct FullPath {
		std::string path;
		std::string filename;
	};

	FullPath split_path_and_filename(std::string path);

	struct Path {

		string full;
		string extension;
		string filename;
		string path;
		Path(const string full_path);
	};
}

#endif // __PathTools_H_INCLUDE__