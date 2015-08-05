#include "ToDoScheduler.h"

int main(int argc, char* argv[]) {
	try{
		ToDoScheduler options(argc, argv);
		options.execute();
	}catch(std::exception &error) {
		std::cerr << error.what(); 
	}
	return 0;
}