//=================================
// include guard
#ifndef __WORLDFACTORY_H_INCLUDED__
#define __WORLDFACTORY_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Tools/StringTools.h"
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
#include "XmlIO/XmlFileIo.h"
#include "TracerException.h"

//------------------------------------------------------------------------------
class WorldFactory : public XmlFileIo{
	
	CartesianFrame* root_of_World;
	std::string absolute_path ="";
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

	void extract_include_path(std::string &path,const pugi::xml_node node);

	void include_file(CartesianFrame* mother,const pugi::xml_node node);

	void fabricate_frame(CartesianFrame* mother,const pugi::xml_node frame_node);

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
		double &x_width, double &y_width, const pugi::xml_node node 
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
		double &Ax, double &Ay, 
		double &Bx, double &By, 
		double &Cx, double &Cy,
		const pugi::xml_node node
	);

	void assert_name_of_child_frame_is_not_in_use_yet(
		const CartesianFrame *mother,
		const std::string name_of_additional_child
	)const;

	bool file_can_be_opened(const char* file)const;
};
#endif // __WORLDFACTORY_H_INCLUDED__ 