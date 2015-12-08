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

	std::string bg_color(const std::string &text, const Color col) {

		std::string out;
		out += "\33[0;48;";
		out += "2;";
		out += std::to_string(col.get_R_as_uchar());
		out += ";";
		out += std::to_string(col.get_G_as_uchar());
		out += ";";
		out += std::to_string(col.get_B_as_uchar());
		out += "m";
		out += text;
		out += "\33[0m";

		std::cout << out << "\n";
		
		return out;
	}

	std::string get_progress_print(const double fraction) {
			
		uint cols = Screen::cols() - 2;

		std::stringstream perc;
		perc.precision(4);
		perc << fraction*100.0 << "/100";
		std::string per = perc.str();

		uint percent_pos = cols - per.length();

		std::stringstream out;
		out << "[";
		for(uint col=0; col<cols; col ++) {

			double progress = double(col)/double(cols);
			if(col >= percent_pos) {

				int access = col-percent_pos;

				if(progress < fraction)
					out << green_font( std::string(1,per.at(access)) );
				else
					out << per.at(access);	
			}else{

				if(progress < fraction)
					out << green_font("|");
				else
					out << " ";	
			}
		}
		out << "]\n";

		return out.str(); 
	}

	std::string overwrite_last_line_with(const std::string &text) {

		std::stringstream out;
		out << "\33[1F";
		out << text;
		return out.str(); 		
	}

	std::string green_font(const std::string &text) {

		std::stringstream out;
		out << "\33[0;32m" << text << "\33[0m";
		return out.str();
	}

	namespace Screen {
	
		uint cols() {

			struct winsize w;
		    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
			return w.ws_col;
		}

		uint rows() {

			struct winsize w;
		    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
			return w.ws_row;
		}
	}
}