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
#include "TracerException.h"
#include "XmlIoException.h"
#include "XmlFileIo.h"

//------------------------------------------------------------------------------
class tuple3{
public:
	double x;
	double y;
	double z;
tuple3(){ x=0.0; y=0.0; z=0.0; }	
};
//------------------------------------------------------------------------------
class WorldFactory : public XmlFileIo{
	
	CartesianFrame* root_of_World;
	std::string absolute_path;
	bool verbose;

public:
	WorldFactory();
	void load(std::string path);
	CartesianFrame* world();
private:
	void load_file(
		CartesianFrame* mother,
		std::string path,
		std::string filename
	);

	void extract_path(std::string &path,const pugi::xml_node node);

	void include_file(CartesianFrame* mother,const pugi::xml_node node);

	void frame_factory(CartesianFrame* mother,const pugi::xml_node frame_node);

	void go_on_with_children_of_node(
		CartesianFrame* mother,
		const pugi::xml_node node
	);

	CartesianFrame* produceCartesianFrame(
		CartesianFrame* mother,
		const pugi::xml_node frame_node
	);

	CartesianFrame* producePlane(
		CartesianFrame* mother,
		const pugi::xml_node frame_node
	);

	CartesianFrame* produceSphere(
		CartesianFrame* mother,
		const pugi::xml_node node
	);

	CartesianFrame* produceCylinder(
		CartesianFrame* mother,
		const pugi::xml_node node
	);

	CartesianFrame* produceFactReflector(
		CartesianFrame* mother,
		const pugi::xml_node node
	);

	CartesianFrame* produceDisc(
		CartesianFrame* mother,
		const pugi::xml_node node
	);

	CartesianFrame* produceTriangle(
		CartesianFrame* mother,
		const pugi::xml_node node
	);

	void extract_Frame_props(
		std::string &name,
		Vector3D &position,
		Rotation3D &rotation,
		const pugi::xml_node frame_node
	);

	void extract_Surface_props(
		ReflectionProperties &reflection_cefficient,
		ColourProperties &colour,
		const pugi::xml_node node
	);

	void extract_Plane_props(
		double &min_x, double &max_x, double &min_y, double &max_y,
		const pugi::xml_node node
	);

	void extract_Sphere_props(double &radius,const pugi::xml_node node);

	void extract_Cylinder_props(
		double &cylinder_radius,
		Vector3D &start_of_cylinder,
		Vector3D &end_of_cylinder,
		const pugi::xml_node node
	);

	void extract_FACT_props(double &alpha,const pugi::xml_node node);

	void extract_Disc_props(double &radius,const pugi::xml_node node);

	void extract_Triangle_props(
		Vector3D &point_A,
		Vector3D &point_B,
		Vector3D &point_C,
		const pugi::xml_node node
	);

	void strto3tuple(tuple3 &tuple, const std::string text)const;

	double pedantic_strtod(std::string text_to_parse)const;

	void check_name_for_multiple_usage(
		const CartesianFrame *mother,
		const std::string name_of_additional_child
	)const;

	void check(
		const pugi::xml_node node,
		const std::string child_name,
		const std::string attribute_name
	)const;

	void check(
		const pugi::xml_node node,
		const std::string child_name
	)const;

	void build_offset_data_to_locate_line_and_column(
		std::vector<ptrdiff_t>& result, 
		const char* file
	)const;

	std::pair<int, int> get_line_and_column_location(
		const std::vector<ptrdiff_t>& data,
		ptrdiff_t offset
	)const;

	std::pair<int, int> offste_to_line_and_column(
		const char* file,
		ptrdiff_t offset
	)const;

	bool file_can_be_opened(const char* file)const;
};
#endif // __WORLDFACTORY_H_INCLUDED__ 