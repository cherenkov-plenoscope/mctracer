//=================================
// include guard
#ifndef __PathTools_H_INCLUDE__
#define __PathTools_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies 
#include "StringTools.h"

// We try to use the same wording as in python's os.path
namespace PathTools {

	std::string join(std::string path1, std::string path2);

	struct Path {
		std::string path;
		std::string dirname;
		std::string basename;
		std::string basename_wo_extension;
		std::string extension;
		Path(const std::string full_path);
		Path();
	};
}

#endif // __PathTools_H_INCLUDE__