#include "Tools/UserInteraction.h"
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <sstream>
#include "TracerException.h"

namespace UserInteraction {
    
	void print_welcome_screen() {
		std::stringstream out;
		//               1         2         3         4         5         6
		//      123456789012345678901234567890123456789012345678901234567890
		out << " ___________________________________________________________\n";
		out << "|                                                           \n";
		out << "|                	mwelcome to MCtracer                    \n";
		out << "|                                                           \n";
		out << "|        high performance raytracing tool to design,        \n";
		out << "|     investigate and produce Monte Carlo simulations       \n";
		out << "|                 in particle physics                       \n";
		out << "|                                                           \n";
		out << "|     1st Author: Sebastian Achim Mueller                   \n";
		out << "|     2nd Author: Dominik Neise                             \n";
		out << "|                                                           \n";
		out << "|     compiled on: " << TracerException::compile_time <<   "\n";	
		out << "|___________________________________________________________\n";
		std::cout << out.str();	
	}
	
	void ClearScreen() {

		std::cout << std::string( 5, '\n' );
	}

	bool is_Escape_key(const int user_input_key) {

		return user_input_key == 27;
	}
}