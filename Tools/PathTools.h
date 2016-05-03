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

// We try to use the same wording as in python's os.path
namespace PathTools {

	string join(string path1, string path2);

	struct Path {
		string path;
		string dirname;
		string basename;
		string basename_wo_extension;
		string extension;
		Path(const string full_path);
		Path();
	};
}

#endif // __PathTools_H_INCLUDE__