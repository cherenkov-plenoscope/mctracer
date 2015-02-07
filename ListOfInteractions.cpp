#include "ListOfInteractions.h"	
//------------------------------------------------------------------------------
ListOfInteractions::~ListOfInteractions() {
	for(Intersection* intersection : Interactions)
		delete intersection;
}
//------------------------------------------------------------------------------
double ListOfInteractions::get_accumulative_distance()const {
  	
  	double accumulative_distance = 0.0;
	
	for(Intersection* intersection : Interactions) 
      	accumulative_distance += intersection->get_intersection_distance();

    return accumulative_distance;
}
//------------------------------------------------------------------------------
void ListOfInteractions::print()const {
	std::cout << get_print();
}
//------------------------------------------------------------------------------
std::string ListOfInteractions::get_print()const {
	
	std::stringstream out;
	out << "_interaction_history_(" << Interactions.size() << ")_\n";
	for(Intersection* intersection : Interactions)
		out << "| " << intersection->get_print();

	return out.str();	
}
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const ListOfInteractions& intersec_list){
    os << intersec_list.get_print();
    return os;
}
