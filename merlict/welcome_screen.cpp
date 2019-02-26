// Copyright 2014 Sebastian A. Mueller
#include "merlict/welcome_screen.h"
#include <sstream>

namespace merlict {

std::string welcome_screen() {
    std::stringstream out;
    //               1         2         3         4         5         6
    //      123456789012345678901234567890123456789012345678901234567890
    out << "                                                            \n";
    out << "                         mctracer                           \n";
    out << "                                                            \n";
    out << "         propagating photons in complex sceneries           \n";
    out << "                   Astro particle physics                   \n";
    out << "                                                            \n";
    out << "                  Sebastian Achim Mueller                   \n";
    out << "                        ETH Zurich                          \n";
    out << "                                                            \n";
    return out.str();
}

}  // namespace relleums
