#include "ToDoScheduler.h"

int main(int argc, char* argv[]) {
	try{
		ToDoScheduler tasks(argc, argv);
	}catch(std::exception &error) {
		std::cerr << error.what(); 
	}
	return 0;
}