#include "SensorIntersection.h"
//======================================================================
SensorIntersection::~SensorIntersection(){
	std::cout<<"deconstruct sensor hit"<<std::endl;
}
//======================================================================
void SensorIntersection::set_sensor_hit(
	const double new_intersection_point_u,
	const double new_intersection_point_v,
	const double new_distance_source_2_sensor
){
	intersection_point_u = new_intersection_point_u;
	intersection_point_v = new_intersection_point_v;
	distance_from_source_to_sensor_in_m = 
	new_distance_source_2_sensor;
}
//======================================================================
std::string SensorIntersection::get_string() const{
	std::stringstream out;
	out<<"Hit in (u|v): ("<<intersection_point_u;
	out<<"|"<<intersection_point_v;
	out<<") [m], distance to source: ";
	out<<distance_from_source_to_sensor_in_m<<" [m]"<<std::endl;
	return out.str();
}
//======================================================================
std::string SensorIntersection::get_csv_string() const{
	std::stringstream out;
	out<<std::setprecision(9)<<intersection_point_u<<",";
	out<<std::setprecision(9)<<intersection_point_v<<",";
	out<<std::setprecision(15)
	<<distance_from_source_to_sensor_in_m<<std::endl;
	return out.str();
}
//======================================================================
void SensorIntersection::disp() const{
	std::cout<<get_string();
}
