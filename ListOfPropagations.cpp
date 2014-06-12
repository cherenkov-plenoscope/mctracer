#include "ListOfPropagations.h"
//==================================================================
ListOfPropagations::ListOfPropagations(std::string new_name_for_list_of_propagations){

	name_of_list_of_propagations = new_name_for_list_of_propagations;
}
//==================================================================
void ListOfPropagations::push_back(Ray* ptr_to_ray_to_push_back){
	list_of_ptrs_to_propagations.push_back(ptr_to_ray_to_push_back);
}
//==================================================================
void ListOfPropagations::export_csv(
	std::string name_of_csv_file_to_be_exported,
	GlobalSettings& settings
)const{
	
	ofstream	csv_output_file;
	csv_output_file.open(name_of_csv_file_to_be_exported);

	for(Ray* single_ray : list_of_ptrs_to_propagations){
		CsvRow row =  single_ray->getCsvRow(settings);
		csv_output_file << row;
	}	

	csv_output_file.close();
}
//==================================================================
void ListOfPropagations::disp()const{
	std::cout << get_info_string();
}
//======================================================================
std::string ListOfPropagations::get_info_string()const{
	std::stringstream out;
	
	out << " _____List_of_Rays_____\n";
	out << "| name: " << name_of_list_of_propagations << "\n";
	out << "| number of Rays: " << list_of_ptrs_to_propagations.size() << "\n";
	out << "| files fed into this list:\n";
	if(list_of_files_fed_into_list.size() == 0){
		out << "| No file added yet\n";	
	}else{
		for (std::string a_single_name : list_of_files_fed_into_list){
			out << "| " << a_single_name << "\n";
		}
	}
	out << "|______________________\n";
	
	return out.str();
}
//======================================================================
void ListOfPropagations::propagate(	
	const CartesianFrame* world, 
	//ListOfInteractions* history,
	const GlobalSettings* settings
){

	unsigned long long i;

	#pragma omp parallel shared(settings,world)
	{	

		#pragma omp for schedule(dynamic) private(i) 
		for(i = 0LL; i<list_of_ptrs_to_propagations.size(); i++ ){

			ListOfInteractions* history_of_this_specific_ray;
			history_of_this_specific_ray = new ListOfInteractions;

			list_of_ptrs_to_propagations.at(i) -> 
			set_history(history_of_this_specific_ray);

			list_of_ptrs_to_propagations.at(i) -> propagate(
				world,
				history_of_this_specific_ray,
				0,
				NULL,
				settings
			);

		}
	}

}
//==================================================================
void ListOfPropagations::export_history_csv(
	std::string name_of_csv_file_to_be_exported,
	GlobalSettings& settings,
	std::string options
)const{
	
	ofstream	csv_output_file;
	csv_output_file.open(name_of_csv_file_to_be_exported);

	for(Ray* single_ray : list_of_ptrs_to_propagations){
		CsvRow row =  single_ray->get_history()->getCsvRow(settings,options);
		csv_output_file << row;
	}	

	csv_output_file.close();
}
//======================================================================
// friends of osstream
//======================================================================
std::ostream& operator<<(
	std::ostream& os, const ListOfPropagations& list){
    os << list.get_info_string();
    return os;
}
