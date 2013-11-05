//=================================
// include guard
#ifndef __WORLDFACTORY_H_INCLUDED__
#define __WORLDFACTORY_H_INCLUDED__

//=================================
// forward declared dependencies


//=================================
// included dependencies
#include <iostream> 
#include <string>
#include <sstream>
#include "CartesianFrame.h"
#include "Triangle.h"
#include "Plane.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Disc.h"
#include "FactTelescope.h"
#include "Vector3D.h"
#include "Rotation3D.h"
#include "ReflectionProperties.h"
#include "ColourProperties.h"
// XML parser library 
#include "pugixml.hpp"

// trivial boos logging
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

class tuple3{
public:
	double x;
	double y;
	double z;
tuple3(){ x=0.0; y=0.0; z=0.0; }	
};

//=================================
class WorldFactory   {
	
	CartesianFrame* root_of_World;
	
public:
WorldFactory();
//=================================
bool load_file(std::string filename);
//=================================
CartesianFrame* get_pointer_to_world();
//=================================
private:
void frame_factory
(CartesianFrame* mother,const pugi::xml_node frame_node);
//=================================
void go_on_with_children_of_node
(CartesianFrame* mother,const pugi::xml_node node);
//=================================
CartesianFrame* produceCartesianFrame
(CartesianFrame* mother,const pugi::xml_node frame_node);
//=================================
CartesianFrame* producePlane
(CartesianFrame* mother,const pugi::xml_node frame_node);
//=================================
CartesianFrame* produceSphere
(CartesianFrame* mother,const pugi::xml_node node);
//=================================
CartesianFrame* produceCylinder
(CartesianFrame* mother,const pugi::xml_node node);
//=================================
CartesianFrame* produceFactReflector
(CartesianFrame* mother,const pugi::xml_node node);
//=================================
CartesianFrame* produceDisc
(CartesianFrame* mother,const pugi::xml_node node);
//=================================
CartesianFrame* produceTriangle
(CartesianFrame* mother,const pugi::xml_node node);
//=================================
bool parse3tuple(tuple3 &tuple,const std::string text);
//=================================
bool extract_frame(std::string &name,Vector3D &position,
Rotation3D &rotation,const pugi::xml_node frame_node);
//=================================
bool set_surface(ReflectionProperties &reflection_cefficient,
ColourProperties &colour,const pugi::xml_node node);
//=================================
bool set_plane(double &min_x, double &max_x, double &min_y, double &max_y,
const pugi::xml_node node);
//=================================
bool set_sphere(double &radius,const pugi::xml_node node);
//=================================
bool set_Cylinder(double &cylinder_radius,Vector3D &start_of_cylinder,
Vector3D &end_of_cylinder,const pugi::xml_node node);
//=================================
bool set_fact_reflector(double &alpha,const pugi::xml_node node);
//=================================
bool set_Disc(double &radius,const pugi::xml_node node);
//=================================
bool set_Triangle(
Vector3D &point_A,
Vector3D &point_B,
Vector3D &point_C,
const pugi::xml_node node);
//=================================
bool check_name_for_multiple_usage(const 
CartesianFrame *entitie_in_world,std::string name);
//=================================
};

#endif // __WORLDFACTORY_H_INCLUDED__ 
