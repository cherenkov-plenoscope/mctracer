// Copyright 2014 Sebastian A. Mueller
#include "Tools/UserInteraction.h"
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <string>
#include "Tools/StringTools.h"

namespace relleums {
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

}  // namespace UserInteraction
}  // namespace relleums
