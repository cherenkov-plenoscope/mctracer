//=================================
// include guard
#ifndef __RAY_H_INCLUDED__
#define __RAY_H_INCLUDED__

//=================================
// forward declared dependencies
//class CartesianFrame;
class HomoTrafo3D;
//class Intersection;
class GlobalSettings;
//=================================
// included dependencies
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>
#include <algorithm>
#include "Vector3D.h"
#include "ColourProperties.h"
#include "CartesianFrame.h"
#include "Intersection.h"
#include "GlobalSettings.h"
//=================================
class Ray{
protected:
Vector3D base;
Vector3D dir;
public:
//======================================================================
virtual std::string get_csv_line()const;
virtual std::string get_csv_line(uint decimal_precision)const;
//======================================================================
void set_ray(const Vector3D nbase,const Vector3D ndir);
//======================================================================
void set_base(const double x,const double y,const double z);
//======================================================================
void set_base(const Vector3D nbase);
//======================================================================
void set_dir(const double x,const double y,const double z);
//======================================================================
void set_dir(const Vector3D ndir);
//======================================================================
Vector3D get_support()const;
//======================================================================
Vector3D get_direction()const;
//======================================================================
void disp()const;
//======================================================================
std::string get_string()const;
//======================================================================
Vector3D get_position_on_ray(const double scalar)const;
//======================================================================
void pre_trace(
const CartesianFrame* frame_to_check_for_interaction_of_ray_and_max_sphere, 
std::vector<const CartesianFrame*> *Ptr2ListOfFramesWithIntersectionsOfRayAndMaxSpehre
)const;
//======================================================================
bool is_ray_hitting_frame(const CartesianFrame* frame)const;
//======================================================================
void disp_possible_hit_list(const CartesianFrame *frame)const;
//======================================================================
void operator= (Ray ray);
//======================================================================
void homo_transformation_of_ray(Ray* ray,const HomoTrafo3D *T)const;
//======================================================================
void test_intersection_for_hit_candidates(
std::vector<const CartesianFrame*> *list_of_objects_which_might_intersect,
std::vector<const CartesianFrame*> *list_of_intersecting_objects,
std::vector<Intersection*> 
*ptr_to_list_of_ptr_to_intersections_which_might_take_place,
std::vector<Intersection*> *ptr_to_list_of_ptr_to_intersections,
const CartesianFrame* object_reflected_from,
int refl_count
)const;
//======================================================================
void calculate_reflected_ray(	
Intersection * pointer_to_closest_intersection,
Ray *ray_reflection_on_object);
//======================================================================
Intersection* calculate_closest_intersection(	
		Intersection *pointer_to_closest_intersection,
		std::vector<Intersection*> *pointer_to_list_of_intersections)const;
//======================================================================
ColourProperties trace(const CartesianFrame* world,
				int refl_count,
				const CartesianFrame* object_reflected_from,
				GlobalSettings *settings);
//======================================================================
/*
void trace_science(CartesianFrame* world,
				int refl_count,
				const CartesianFrame* object_reflected_from,
				GlobalSettings *settings,
				double dbl_passed_distance_from_source_to_sensor);*/
//======================================================================
double get_distance_to_closest_object(const CartesianFrame* world,
				int refl_count,
				CartesianFrame* object_reflected_from,
				const GlobalSettings *settings,
				double dbl_passed_distance_from_source_to_sensor)const;
//======================================================================
bool operator() (Intersection* one, Intersection* two)const;
//======================================================================
friend std::ostream& operator<<(std::ostream& os, const Ray& ray_to_be_displayed);
};
#endif // __RAY_H_INCLUDED__ 
