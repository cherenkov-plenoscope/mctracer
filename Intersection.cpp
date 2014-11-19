#include "Intersection.h"
//==================================================================
const CartesianFrame * Intersection::get_pointer_to_intersecting_object(
)const{
	return ptr_to_intersecting_object;
}
//==================================================================
void Intersection::set_pointer_to_intersecting_object(
	const CartesianFrame* new_ptr_to_intersectiong_object
){
	ptr_to_intersecting_object=new_ptr_to_intersectiong_object;
}
//==================================================================
Intersection::Intersection(){
	ptr_to_intersecting_object=NULL;
	flag_intersection = false;
}
//==================================================================
void Intersection::set_intersection_flag(
	const bool flag_new_intersection
){
	flag_intersection = flag_new_intersection;
}
//==================================================================
void Intersection::set_intersection(
	const Vector3D* ptr_to_intersection_vector,
	const Vector3D* ptr_to_intersection_object_surface_normal_vector,
	const double* ptr_to_distance_of_ray 
){
		
	intersection_point = 
	*ptr_to_intersection_vector;
	
	surfacenormal_in_intersection_point= 
	*ptr_to_intersection_object_surface_normal_vector;
	
	distance_of_ray_in_m = 
	*ptr_to_distance_of_ray;
}
//==================================================================
bool Intersection::get_intersection_flag()const{
	return flag_intersection;
}
//==================================================================
void Intersection::disp() const{
	std::stringstream out; 
	out << "Intersection:\n";
	out << get_string();
	std::cout << out.str();
}
//==================================================================
std::string Intersection::get_string() const{
	std::stringstream out; 
	out<<"flag_intersection: ";
	if(flag_intersection){out<<"true";}else{out<<"false";}out << std::endl;
	if(flag_intersection){
		out << "intersection vector: ";
		out << intersection_point.get_string() << std::endl;
		out << "intersection object surface normal: ";
		out << surfacenormal_in_intersection_point.get_string() << std::endl;
		out << "Distance passed by ray: ";
		out << distance_of_ray_in_m << " [m]"<< std::endl;
		out << "Object: ";
		if( ptr_to_intersecting_object == NULL ){
			out<<"NULL";
		}else{
			out<< ptr_to_intersecting_object->get_name_of_frame();
		}
	}
	return  out.str();
}
//==================================================================
double Intersection::get_intersection_distance()const{
	return distance_of_ray_in_m;
}
//==================================================================
void Intersection::get_reflection_direction_in_object_system(
	Vector3D* vec
)const{
	surfacenormal_in_intersection_point.mirror(vec);
}
//==================================================================
void Intersection::get_intersection_vec_in_object_system(
	Vector3D *inter
)const{ 	
	*inter = intersection_point;
}
//==================================================================
CsvRow Intersection::getCsvRow(GlobalSettings &settings)const{
	CsvRow row;

	CsvRow row_intersection_vector = intersection_point.getCsvRow(settings);

	//-------------- build  csv entry of double DoF -------------
	CsvRow row_Distance_of_Flight;
	stringstream DoF;
	DoF.precision(settings.get_decimal_precision_for_csv_output());
	DoF << distance_of_ray_in_m;

	row_Distance_of_Flight.push_back( DoF.str() );
	//-----------------------------------------------------------

	row.append(row_intersection_vector);
	row.append(row_Distance_of_Flight);

	return row;
}