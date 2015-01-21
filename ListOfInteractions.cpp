#include "ListOfInteractions.h"	
//----------------------------------------------------------------------
ListOfInteractions::~ListOfInteractions(){
	for(Intersection* an_Intersection : Interactions){
		delete an_Intersection;
	}
}
//----------------------------------------------------------------------
void ListOfInteractions::show()const{

	std::cout << "___List of " << Interactions.size() << " interactions:____" << endl;

	for(Intersection* an_Intersection : Interactions){
		std::cout << an_Intersection->get_string() << "\n";
		std::cout << "_____________" << endl;
	}

	std::cout << "______________________________" << endl;
}
//======================================================================
/*CsvRow ListOfInteractions::getCsvRow(GlobalSettings& settings)const{
	
	// only the last intersection
	return Interactions.at(Interactions.size() - 1) -> getCsvRow( settings );
}*/
//======================================================================
double ListOfInteractions::get_accumulative_distance()const{
  	
  	double accumulative_distance = 0.0;
	
	for(Intersection* an_Intersection : Interactions){
      accumulative_distance += an_Intersection->get_intersection_distance();
    }	

    return accumulative_distance;
}