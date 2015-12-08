//=================================
// include guard
#ifndef __UserInteraction_H_INCLUDED__
#define __UserInteraction_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <string>
#include "Core/Color.h"

namespace UserInteraction {
	
	void print_welcome_screen();	
	void ClearScreen();
	bool is_Escape_key(const int user_input_key);
	std::string bg_color(const std::string &text, const Color col);
	std::string get_progress_print(const double fraction);
	std::string overwrite_last_line_with(const std::string &text);
	std::string green_font(const std::string &text);
	namespace Screen {
		uint cols();
		uint rows();
	}
}
#endif // __UserInteraction_H_INCLUDED__