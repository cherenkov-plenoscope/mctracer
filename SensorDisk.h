//=================================
// include guard
#ifndef __SENSORDISK_H_INCLUDED__
#define __SENSORDISK_H_INCLUDED__

//=================================
// forward declared dependencies
class SensorIntersection;
//=================================
// included dependencies
#include <iostream> 
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "Disk.h"
#include "SensorIntersection.h"

//=================================
class SensorDisk :public Disk{

	std::vector<SensorIntersection*> list_of_hits;
public:
	//==================================================================
	SensorDisk();
	//==================================================================
	~SensorDisk();
	//==================================================================
	/*
	void push_back_intersection(
	const Intersection* intersection_information,
	double dbl_new_complete_passed_distance)*/
	//==================================================================	
	void push_back_SensorIntersection
	(SensorIntersection *pointer_to_new_sensor_hit);
	//==================================================================
	std::string get_string()const;
	//==================================================================
	std::string get_full_string()const;
	//==================================================================
	std::string get_csv_string()const;
	//==================================================================
	void disp()const;
	//==================================================================
	void disp_full_table_of_intersections()const;
	//==================================================================
	void export_csv_table_of_intersections(std::string str_file_name)const;
	//==================================================================
	void clear_intersection_list();
};
#endif // __SENSORDISK_H_INCLUDED__
