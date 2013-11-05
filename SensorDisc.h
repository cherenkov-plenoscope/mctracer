//=================================
// include guard
#ifndef __SENSORDISC_H_INCLUDED__
#define __SENSORDISC_H_INCLUDED__

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
#include "Disc.h"
#include "SensorIntersection.h"

//=================================
class SensorDisc :public Disc{

	std::vector<SensorIntersection*> list_of_hits;
public:
	//==================================================================
	SensorDisc();
	//==================================================================
	~SensorDisc();
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
#endif // __SENSORDISC_H_INCLUDED__
