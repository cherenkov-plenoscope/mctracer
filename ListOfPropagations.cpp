#include "ListOfPropagations.h"
//======================================================================
// friends of osstream
//======================================================================
std::ostream& operator<<(
	std::ostream& os, const ListOfPropagations& list){
    os << list.get_info_string();
    return os;
}
//==================================================================
ListOfPropagations::ListOfPropagations(std::string new_name_for_list_of_propagations){

	name_of_list_of_propagations = new_name_for_list_of_propagations;
}
//==================================================================
void ListOfPropagations::push_back(Ray* ptr_to_ray_to_push_back){
	list_of_ptrs_to_propagations.push_back(ptr_to_ray_to_push_back);
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
	out << "|______________________\n";
	
	return out.str();
}
//======================================================================
void ListOfPropagations::propagate(	
	const CartesianFrame* world, 
	const GlobalSettings* settings
){
	unsigned long long i;

	std::cout << "Propagate list of " << list_of_ptrs_to_propagations.size() << " rays...\n";

	if(settings->MultiThread()){
		// MULTITHREAD

		int number_of_threads, thread_id, ray_counter;
		std::stringstream out;

		#pragma omp parallel shared(settings,world) private(number_of_threads, thread_id, out, ray_counter)
		{	

			PseudoRandomNumberGenerator dice_for_this_thread_only;
			ray_counter = 0;
			thread_id = omp_get_thread_num();
			number_of_threads = omp_get_num_threads();

			#pragma omp for schedule(dynamic) private(i) 
			for(i = 0LL; i<list_of_ptrs_to_propagations.size(); i++ )
			{
				ray_counter++;
				PropagateSingleRay(
					world,settings,
					&dice_for_this_thread_only,
					i
				);
			}

			out << "Thread " << thread_id+1 << "/" << number_of_threads;
			out << " is doing " << ray_counter << "/";
			out << list_of_ptrs_to_propagations.size() << " rays. ";
			out << "Seed: " << dice_for_this_thread_only.seed() << "\n";
			cout << out.str();
		}
	}else{
		// SINGLETHREAD

		PseudoRandomNumberGenerator dice;

		for(i = 0LL; i<list_of_ptrs_to_propagations.size(); i++ )
		{
			PropagateSingleRay(world,settings,&dice,i);
		}
		cout << "Single thread is doing all the rays\n";
	}
}
//==================================================================
void ListOfPropagations::PropagateSingleRay(	
	const CartesianFrame* world, 
	const GlobalSettings* settings,
	PseudoRandomNumberGenerator *dice,
	unsigned long long index
){
	ListOfInteractions* history_of_this_specific_ray;
	history_of_this_specific_ray = new ListOfInteractions;

	list_of_ptrs_to_propagations.at(index) -> 
	SetHistory(history_of_this_specific_ray);

	list_of_ptrs_to_propagations.at(index) -> propagate(
		world,
		history_of_this_specific_ray,
		0,
		NULL,
		settings,
		dice
	);
}
//==================================================================
/*void ListOfPropagations::export_history_csv(
	GlobalSettings& settings,
	std::string name_of_csv_file_to_be_exported
)const{
	
	ofstream	csv_output_file;
	csv_output_file.open(name_of_csv_file_to_be_exported+"_history.csv");

	for(Ray* single_ray : list_of_ptrs_to_propagations){
		CsvRow row =  single_ray->getCsvRowHistory(settings);
		csv_output_file << row;
	}	

	csv_output_file.close();
}*/
//======================================================================
/*void ListOfPropagations::export_history_csv(
		GlobalSettings& settings
)const{
	export_history_csv(settings,name_of_list_of_propagations);
}*/
//==================================================================
/*void ListOfPropagations::export_propagations_csv(
	GlobalSettings& settings
)const{
	ofstream	csv_output_file;
	csv_output_file.open(name_of_list_of_propagations+".csv");

	for(Ray* single_ray : list_of_ptrs_to_propagations){
		CsvRow row =  single_ray->getCsvRow(settings);
		csv_output_file << row;
	}	

	csv_output_file.close();
}*/
//==================================================================
/*void ListOfPropagations::import_propagations_csv(
	std::string name_of_file_to_import,
	GlobalSettings& settings
){
	ifstream csv_input_file( name_of_file_to_import );

	CsvRow SingleRow;

	if( csv_input_file.is_open() ){
   		while ( SingleRow.ReadNextRow( csv_input_file ) )
   			cout << SingleRow << endl;
    	csv_input_file.close();		
	}else{
		cout << "Unable to open file" << endl;
	}
}*/
