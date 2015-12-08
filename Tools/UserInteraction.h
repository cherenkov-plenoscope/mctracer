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
}
#endif // __UserInteraction_H_INCLUDED__