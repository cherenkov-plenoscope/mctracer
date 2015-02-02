#include "Tools.h"
//------------------------------------------------------------------------------
double Deg2Rad(double angle_in_Deg) {
	return (M_PI*angle_in_Deg)/180.0;
}
//------------------------------------------------------------------------------
double Rad2Deg(double angle_in_Rad) {
	return (angle_in_Rad/M_PI)*180.0;
}
//------------------------------------------------------------------------------
namespace UserInteraction {

	std::string input(const std::string request) {
		std::cout << request;
		std::string user_input;
		std::cin  >> user_input;	
		return user_input;
	}
    //--------------------------------------------------------------------------
	void print_welcome_screen() {
		std::stringstream out;
		//               1         2         3         4         5         6
		//      123456789012345678901234567890123456789012345678901234567890
		out << " ___________________________________________________________ \n";
		out << "|                                                           |\n";
		out << "|                 welcome to MCtracer                       |\n";
		out << "|                                                           |\n";
		out << "|      high performance raytracing tool to design and       |\n";
		out << "|     investigate optical devices for particle physics      |\n";
		out << "|                                                           |\n";
		out << "|     Author: Sebastian Mueller              year 2013      |\n";
		out << "|___________________________________________________________|\n";
		std::cout << out.str();	
	}
	//--------------------------------------------------------------------------
	void ClearScreen() {
		std::cout << std::string( 5, '\n' );
	}
	//--------------------------------------------------------------------------
	std::string parse_config_file_path(int number_of_arguments, char* arguments[]) {
		
		/*std::cout << "Have " << number_of_arguments << " arguments:" << std::endl;
	    for (int i = 0; i < number_of_arguments; ++i) {
	        std::cout << arguments[i] << std::endl;
	    }*/

		if(number_of_arguments != 2) {
			std::stringstream info;
			info << "Wrong number of input arguments, expected 1, but actual: ";
			info << number_of_arguments-1 << ".\n";
			throw TracerException(info.str());
		}

		std::string config_path(arguments[1]);
	    return config_path;
	}
	//--------------------------------------------------------------------------
	bool is_Escape_key(const int user_input_key) {
		return user_input_key == 27;
	}
}