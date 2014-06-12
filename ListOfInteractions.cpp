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

	std::cout << "______________________________" << endl;
}
//======================================================================
CsvRow ListOfInteractions::getCsvRow(GlobalSettings& settings,std::string options)const{
	
	// only the last intersection
	return Interactions.back() -> getCsvRow( settings );
}
//======================================================================
double ListOfInteractions::get_accumulative_distance()const{
  	
  	double accumulative_distance = 0.0;
	
	for(Intersection* inter : Interactions){
      accumulative_distance += inter->get_intersection_distance();
    }	

    return accumulative_distance;
}