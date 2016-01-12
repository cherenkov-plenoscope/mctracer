#include "LightFieldTelescope/Propagation.h"

int main(int argc, char* argv[]) {
	try{
		
		LightFieldTelescope::Propagation propagation(argc, argv);
	}catch(std::exception &error) {

		std::cerr << error.what(); 
	}
	return 0;
}