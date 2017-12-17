//=================================
// include guard
#ifndef __FILETOOLS_H_INCLUDED__
#define __FILETOOLS_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies	  
#include <string>


namespace FileTools {
	void write_text_to_file(
		const std::string &text,
		const std::string &path
	);
}

#endif // __FILETOOLS_H_INCLUDED__