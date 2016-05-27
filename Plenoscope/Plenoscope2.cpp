#include "Plenoscope/Plenoscope.h"

namespace Plenoscope {
    
    Plenoscope::Plenoscope(Config cfg):geometry(cfg), factory(&geometry) {
    	std::cout << __LINE__ << "\n";
    	std::cout << geometry.get_print() << "\n";
        sensors = factory.get_sub_pixels();
        std::cout << sensors->size() << "\n";
        std::cout << __LINE__ << "\n";
    }
}