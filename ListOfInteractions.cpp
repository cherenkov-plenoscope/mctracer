#include "ListOfInteractions.h"	
//----------------------------------------------------------------------
ListOfInteractions::~ListOfInteractions(){
	for(Intersection* inter : Interactions){
		delete inter;
	}
}
//----------------------------------------------------------------------
void ListOfInteractions::show()const{

	std::cout << "___List of " << Interactions.size() << " interactions:____" << endl;

	for(Intersection* interact : Interactions){
		std::cout << interact->get_string() << "\n";
		std::cout << "_____________" << endl;
	}

	std::cout << "_____________________________" << endl;
}
//======================================================================
CsvRow ListOfInteractions::getCsvRow(GlobalSettings& settings)const{
	CsvRow combinedRow;
	
	return combinedRow;
}
//======================================================================
double ListOfInteractions::get_accumulative_distance()const{
  	
  	double accumulative_distance = 0.0;
	
	for(Intersection* inter : Interactions){
      accumulative_distance += inter->get_intersection_distance();
    }	

    return accumulative_distance;
}