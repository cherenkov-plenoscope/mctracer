//=================================
// include guard
#ifndef __LISTOFPROPAGATIONS_H_INCLUDED__
#define __LISTOFPROPAGATIONS_H_INCLUDED__

//=================================
// forward declared dependencies
class GlobalSettings;
//=================================
// included dependencies
#include <iostream>
#include <omp.h>
#include "Ray.h"
#include "CsvHandler.h"
#include <list>
#include <sstream>
using namespace std;

//=================================
class ListOfPropagations{
	
	std::string 				name_of_list_of_propagations;
	std::vector< Ray* > 		list_of_ptrs_to_propagations;

	//-----------------
	std::string get_info_string()const;
	//-----------------
	void PropagateSingleRay(	
		const CartesianFrame* world, 
		const GlobalSettings* settings,
		unsigned long long index
	);
public:
	ListOfPropagations(std::string new_name_for_list_of_propagations);
	//-----------------
	void push_back(Ray* ptr_to_ray_to_push_back);
	//-----------------
	void disp()const;
	//-----------------
	void propagate(	
		const CartesianFrame* world, 
		const GlobalSettings* settings
	);
	//-----------------
	/*
	void export_propagations_csv(
		GlobalSettings& settings
	)const;
	*/
/*
	void import_propagations_csv(
	std::string name_of_file_to_import,
	GlobalSettings& settings
	);
*/	
	
	

/*'
	void export_history_csv(
		GlobalSettings& settings,
		std::string name_of_csv_file_to_be_exported
	)const;

	void export_history_csv(
		GlobalSettings& settings
	)const;
*/
	friend std::ostream& operator<<(std::ostream& os,const ListOfPropagations& list);
};
#endif // __LISTOFPROPAGATIONS_H_INCLUDED__
