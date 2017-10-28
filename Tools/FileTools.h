//=================================
// include guard
#ifndef __FILETOOLS_H_INCLUDED__
#define __FILETOOLS_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies	  
#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <vector>
#include <fstream>

namespace FileTools {
	bool can_be_opened(const std::string &file_path);	
	uint size_in_bytes(const std::string name_of_file_to_get_size_of);
	void write_text_to_file(
		const std::string &text,
		const std::string &path
	);
}

#endif // __FILETOOLS_H_INCLUDED__