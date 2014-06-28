#include "SensorDisc.h"
//==================================================================
SensorDisc::SensorDisc(){
	reflection.SetReflectionCoefficient(0.0);
}
//==================================================================
SensorDisc::~SensorDisc(){
	std::cout<<"Sensor-Disc -> "<<name_of_frame;
	std::cout<<" -> deconstruct"<<std::endl;
	list_of_hits.clear();
}
//==================================================================
/*
void SensorDisc::push_back_intersection(
const Intersection* intersection_information,
double dbl_new_complete_passed_distance){
	
	SensorIntersection *new_sensor_hit;
	new_sensor_hit = new SensorIntersection;
	
	Vector3D vec_intersection_in_object_system;
	intersection_information->
	get_intersection_vec_in_object_system(
	&vec_intersection_in_object_system);
	
	new_sensor_hit->set_sensor_hit(
	vec_intersection_in_object_system.get_x(),
	vec_intersection_in_object_system.get_y(),
	dbl_new_complete_passed_distance);
	
	list_of_hits.push_back(new_sensor_hit);
}*/
//==================================================================	
void SensorDisc::push_back_SensorIntersection
(SensorIntersection *pointer_to_new_sensor_hit){
	list_of_hits.push_back(pointer_to_new_sensor_hit);
}
//==================================================================
std::string SensorDisc::get_string()const{
	std::stringstream out;
	out<<"Sensor_Disc: Number of hits: ";
	out<<list_of_hits.size()<<std::endl;
	return out.str();
}	
//==================================================================
std::string SensorDisc::get_full_string()const{
	std::stringstream out;
	out<<"Sensor_Disc: list of hits"<<std::endl;
	for(unsigned int i=0; i<list_of_hits.size(); i++)
	{
	out<<(i+1)<<": "<<list_of_hits.at(i)->get_string();
	}
	return out.str();
}
//==================================================================
std::string SensorDisc::get_csv_string()const{
	std::stringstream out;
	//out<<"% Sensor_Disc FACT raytracing"<<endl;
	//out<<"% x [m], y [m], d [m];"<<endl;
	for(unsigned int i=0; i<list_of_hits.size(); i++)
	{
	out<<list_of_hits.at(i)->get_csv_string();
	}
	return out.str();
}
//==================================================================
void SensorDisc::disp()const{
	std::cout<<get_string();
}
//==================================================================
void SensorDisc::disp_full_table_of_intersections()const{
	std::cout<<get_full_string();
}
//==================================================================
void SensorDisc::export_csv_table_of_intersections(std::string str_file_name)const{
std::ofstream myfile (str_file_name.c_str());
if (myfile.is_open())
{
	myfile << get_csv_string();
	myfile.close();
}		
	else
	{
		std::cout<<"Sensor-Disc->export_csv_table_of_intersections():";
		std::cout<<" Unable to open file: "<<str_file_name<<std::endl;
	}
}
//==================================================================
void SensorDisc::clear_intersection_list(){

	list_of_hits.clear();
	std::cout<<"Sensor-Disc->clear_intersection_list()"<<std::endl;
	std::cout<<"New intersection count: "<<list_of_hits.size()<<std::endl;
}
