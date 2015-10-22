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
#include <vector>
#include <stdlib.h> 
#include "Core/Frame.h"
#include "Triangle.h"
#include "Plane.h"
#include "Geometry/Sphere.h"
#include "Cylinder.h"
#include "Disc.h"
#include "SegmetedReflectorGenerator.h"
#include "Core/Vector3D.h"
#include "Core/Rotation3D.h"
#include "Core/ReflectionProperties.h"
#include "Core/Color.h"
#include "XmlIO/XmlFileIo.h"
#include "TracerException.h"
#include "PhotonSensor.h"
#include "TelescopeArrayControl.h"
#include "SphereCapWithHexagonalBound.h"
#include "SphereCapWithCylinderBound.h"
#include "BiConvexLensHexBound.h"
#include "FunctionFactory.h"

//------------------------------------------------------------------------------
class WorldFactory : public XmlFileIo{
	
	// resources from xml file
	Frame* root_of_World;
	std::vector<PhotonSensor*> *sensors;
	TelescopeArrayControl* telescopes;
	
	// intternal
	std::string absolute_path ="";
public:
	FunctionFactory *functions;

	WorldFactory();
	void load(std::string path);
	Frame* world();
	TelescopeArrayControl* get_telescope_array_control()const;
	std::vector<PhotonSensor*>* sensors_in_world()const;
private:
	void extract_function_from(const pugi::xml_node node);
	void add_to_sensors_if_sensitive(const pugi::xml_node node, Frame* frame);
	void add_to_array_if_telescope(const pugi::xml_node node, Frame* frame);
	void load_file(Frame* mother, std::string path, std::string filename);
	void extract_include_path(std::string &path,const pugi::xml_node node);
	void include_file(Frame* mother,const pugi::xml_node node);
	void fabricate_frame(Frame* mother,const pugi::xml_node frame_node);
	void go_on_with_children_of_node(
		Frame* mother,
		const pugi::xml_node node
	);

	Vector3D extract_vector_with_name_in_node(
		const std::string name,
		const pugi::xml_node node
	);

	Frame* produceFrame(Frame* mother, const pugi::xml_node frame_node);

	Frame* producePlane(
		Frame* mother,
		const pugi::xml_node frame_node
	);

	Frame* produce_sphere_cap_hexagonal(
		Frame* mother,
		const pugi::xml_node node
	);

	Frame* produceSphere(
		Frame* mother,
		const pugi::xml_node node
	);

	Frame* produceCylinder(
		Frame* mother,
		const pugi::xml_node node
	);

	Frame* produceReflector(
		Frame* mother,
		const pugi::xml_node node
	);

	Frame* produceDisc(
		Frame* mother,
		const pugi::xml_node node
	);

	Frame* produceTriangle(
		Frame* mother,
		const pugi::xml_node node
	);

	Frame* produceBiConvexLensHex(
		Frame* mother,
		const pugi::xml_node node
	);

	RefractiveIndex* extract_medium(
		const pugi::xml_node node
	);

	void extractBiConvexLensHex(
		double &curv_radius, double &outer_radius, const pugi::xml_node node
	);


	void extract_rotation_props(
		const Vector3D &position, 
		Rotation3D &rotation, 
		const pugi::xml_node node
	);	

	const Color* extract_color(const pugi::xml_node node);

	const ReflectionProperties* extract_reflection(const pugi::xml_node node);

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

	void extract_Disc_props(double &radius, const pugi::xml_node node);

	void extract_Triangle_props(
		double &Ax, double &Ay, 
		double &Bx, double &By, 
		double &Cx, double &Cy,
		const pugi::xml_node node
	);

	void assert_name_of_child_frame_is_not_in_use_yet(
		const Frame *mother,
		const std::string name_of_additional_child
	)const;

	void extract_sphere_cap_hexagonal(
		double &focal_length, double &outer_radius, const pugi::xml_node node
	);

	double extract_reflector(
		const std::string key, const pugi::xml_node node
	);

	bool file_can_be_opened(const char* file)const;
};
#endif // __WORLDFACTORY_H_INCLUDED__ 