//=================================
// include guard
#ifndef __SENSORINTERACTION_H_INCLUDED__
#define __SENSORINTERACTION_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip> 

//=================================
// the actual class
class SensorIntersection {
// This class stores information of intersections/hits of a ray and 
// a 2D cartesian (u,v) sensor object.
private:
double intersection_point_u;
double intersection_point_v;
double distance_from_source_to_sensor_in_m;
public:
//==================================================================
~SensorIntersection();
//==================================================================
void set_sensor_hit(
	const double new_intersection_point_u,
	const double new_intersection_point_v,
	const double new_distance_source_2_sensor
);
//==================================================================
std::string get_string() const;
//==================================================================
std::string get_csv_string() const;
//==================================================================
void disp() const;
};
#endif // __SENSORINTERACTION_H_INCLUDED__ 
