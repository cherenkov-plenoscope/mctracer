#include "Tools/UserInteraction.h"
#include "Tools/StringTools.h"
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <sstream>
#include "Xml/BasicAttributes.h"

namespace UserInteraction {
    
    

    void print_welcome_screen() {
        std::stringstream out;
        //               1         2         3         4         5         6
        //      123456789012345678901234567890123456789012345678901234567890
        out << " ___________________________________________________________\n";
        out << "|                                                           \n";
        out << "|                        MCtracer                           \n";
        out << "|                                                           \n";
        out << "|        high performance raytracing tool to design,        \n";
        out << "|    investigate and produce Monte Carlo simulations in     \n";
        out << "|                  Astro particle physics                   \n";
        out << "|                                                           \n";
        out << "|     1st Author: Sebastian Achim Mueller                   \n";
        out << "|     2nd Author: Dominik Neise                             \n";
        out << "|                                                           \n";  
        out << "|___________________________________________________________\n";
        std::cout << out.str(); 
    }
    
    void ClearScreen() {

        std::cout << std::string( 5, '\n' );
    }

    bool is_Escape_key(const int user_input_key) {

        return user_input_key == 27;
    }

    Vec3 get_Vec3() {

        Tuple3 vec;
        std::string input;

        while (true) {

            std::cout << "Enter 3D vector: '[x,y,z]'\n>";
            getline(std::cin, input);

            try{
                vec = StringTools::to_Tuple3(input);
                break;
            }catch(...) {

            }
        }

        return Vec3(vec.x, vec.y, vec.z);
    }
}