#include "WorldFactory.h"
#include "Vector3DFactory.h"
#include "Rotation3DFactory.h"
#include "FrameFactory.h"
#include "Core/Function/LinInterpolFunction.h"
#include "Tools/AsciiIo.h"
#include "Geometry/StereoLitographyIo.h"
//------------------------------------------------------------------------------
WorldFactory::WorldFactory(){

	root_of_World = new Frame(
		"world",
		Vector3D::null,
		Rotation3D::null
	);

	telescopes = new TelescopeArrayControl();
	sensors = new std::vector<PhotonSensor::Sensor*>;
	functions = new FunctionFactory(this);
} 
//------------------------------------------------------------------------------
void WorldFactory::load(std::string path){

	// determine directory
	int position_in_path = path.find_last_of("/\\");

	if(position_in_path == -1)
		absolute_path = "";
	else
		absolute_path = path.substr(0,position_in_path + 1); 

	std::string filename = path.substr(position_in_path + 1);
	
   	load_file(root_of_World, absolute_path, filename);
}
//------------------------------------------------------------------------------
void WorldFactory::load_file(
	Frame* mother,
	std::string path,
	std::string filename
){
	std::string path_of_file_to_load = path  + filename;

	//remember this file until the next is parsed
	XmlName = path_of_file_to_load;
	XmlRelativePath = path;

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path_of_file_to_load.c_str());
	
    XmlResult = result;

    if(result){

		fabricate_frame(mother,doc);	
    }else{
    	std::stringstream info;
		info << "WorldFactory::" << __func__ << "()";
		throw XmlIoException(info.str(), this);
	}
}
//------------------------------------------------------------------------------
void WorldFactory::include_file(
	Frame* mother, const pugi::xml_node node
){
	std::string relative_path_to_xml_to_be_included;
	extract_include_path(relative_path_to_xml_to_be_included, node);

	WorldFactory fab;
	fab.load(absolute_path + relative_path_to_xml_to_be_included);

	Frame *sub_world;
	sub_world = fab.world();

	mother->take_children_from(sub_world);
}
//------------------------------------------------------------------------------
void WorldFactory::extract_include_path(
	std::string &path,const pugi::xml_node node
){
	assert_attribute_exists(node, "path");

	path = node.attribute("path").value();	
}
//------------------------------------------------------------------------------
void WorldFactory::add_to_array_if_telescope(const pugi::xml_node node, Frame* frame) {
	std::string telescope_key = "set_telescope";

	if(has_child(node, telescope_key))
		telescopes->add_to_telescope_array(frame);
}
//------------------------------------------------------------------------------
void WorldFactory::add_to_sensors_if_sensitive(const pugi::xml_node node, Frame* frame) {
	std::string sensor_key = "set_sensitive";

	if(has_child(node, sensor_key)) {
		const pugi::xml_node sensi = node.child(sensor_key.c_str());

		assert_attribute_exists(sensi, "id");
		uint id = uint(StrToDouble(sensi.attribute("id").value())); 
		PhotonSensor::PerfectSensor* sens = new PhotonSensor::PerfectSensor(id, frame);
		sensors->push_back(sens);
	}
}
//------------------------------------------------------------------------------
void WorldFactory::extract_function_from(const pugi::xml_node node) {
	functions->extract_function_from(node);
}
//------------------------------------------------------------------------------
void WorldFactory::fabricate_frame(
Frame* mother,const pugi::xml_node node){
	
	XmlNode = node;
	if(StringTools::is_equal(node.name(),"frame")){
		mother = produceFrame(mother,node);

	}else if(StringTools::is_equal(node.name(),"triangle")){
		mother = produceTriangle(mother,node);
		
	}else if(StringTools::is_equal(node.name(),"plane")){
		mother = producePlane(mother,node);
		
	}else if(StringTools::is_equal(node.name(),"sphere")){
		mother = produceSphere(mother,node);	

	}else if(StringTools::is_equal(node.name(),"cylinder")){
		mother = produceCylinder(mother,node);	

	}else if(StringTools::is_equal(node.name(),"disc")){
		mother = produceDisc(mother,node);	
	
	}else if(StringTools::is_equal(node.name(),"segmented_reflector")){
		mother = produceReflector(mother,node);

	}else if(StringTools::is_equal(node.name(),"bi_convex_lens_hex")){
		mother = produceBiConvexLensHex(mother,node);	

	}else if(StringTools::is_equal(node.name(),"sphere_cap_hexagonal")){
		mother = produce_sphere_cap_hexagonal(mother,node);

	}else if(StringTools::is_equal(node.name(),"stl")){
		mother = produce_stl_object(mother,node);		

	}else if(StringTools::is_equal(node.name(),"include")){
		include_file(mother,node);

	}else if(StringTools::is_equal(node.name(),"function")){
		extract_function_from(node);
		
	}else if( mother->has_mother() ){	
		std::stringstream info;
		info << "WorldFactory::" << __func__ << "() found an unknown item.";
		throw UnknownItem( info.str(), this ,node.name());
	}
	
	add_to_sensors_if_sensitive(node, mother);
	add_to_array_if_telescope(node, mother);
	go_on_with_children_of_node(mother,node);
}
//------------------------------------------------------------------------------
void WorldFactory::go_on_with_children_of_node(
	Frame* mother,const pugi::xml_node node
) {
	// go on with children of node
	for(
		pugi::xml_node sub_node = node.first_child(); 
		sub_node; 
		sub_node = sub_node.next_sibling()
	){	
		std::string sub_node_name = sub_node.name();

		if 		(StringTools::is_equal(sub_node_name,"include")){

			fabricate_frame(mother,sub_node);
		}else if(StringTools::is_equal(sub_node_name,"function")){
			//std::cout << sub_node_name << "\n";
			fabricate_frame(mother,sub_node);
		}else if(StringTools::is_equal(sub_node_name,"frame")){

			fabricate_frame(mother,sub_node);
		}else if(StringTools::is_equal(sub_node_name,"telescope")){

			fabricate_frame(mother,sub_node);
		}else if(StringTools::is_equal(sub_node_name,"triangle")){

			fabricate_frame(mother,sub_node);
		}else if(StringTools::is_equal(sub_node_name,"plane")){

			fabricate_frame(mother,sub_node);
		}else if(StringTools::is_equal(sub_node_name,"sphere")){

			fabricate_frame(mother,sub_node);
		}else if(StringTools::is_equal(sub_node_name,"cylinder")){

			fabricate_frame(mother,sub_node);
		}else if(StringTools::is_equal(sub_node_name,"disc")){

			fabricate_frame(mother,sub_node); 
		}else if(StringTools::is_equal(sub_node_name,"segmented_reflector")){

			fabricate_frame(mother,sub_node);
		}else if(StringTools::is_equal(sub_node_name,"bi_convex_lens_hex")){

			fabricate_frame(mother,sub_node);
		}else if(StringTools::is_equal(sub_node_name,"sphere_cap_hexagonal")){
			
			fabricate_frame(mother,sub_node);
		}else if(StringTools::is_equal(sub_node_name,"stl")){
			fabricate_frame(mother,sub_node);
		}
	}	
}
//------------------------------------------------------------------------------
Frame* WorldFactory::produceFrame(
	Frame* mother,
	const pugi::xml_node node
){
	FrameFactory frameFab(node);
	
	Frame *frame;
	frame = new Frame(
		frameFab.get_name(), 
		frameFab.get_position(), 
		frameFab.get_rotation()
	);

	mother->set_mother_and_child(frame);
	return frame;
}
//------------------------------------------------------------------------------
Frame* WorldFactory::producePlane(
	Frame* mother,const pugi::xml_node node
){
	FrameFactory frameFab(node);
	assert_child_exists(node, "set_surface");
	assert_child_exists(node, "set_plane");

	const Color*		color;
	const Function::Func1D* reflection_vs_wavelength;
	double 					x_width, y_width;
	
	color = extract_color(node.child("set_surface"));
	reflection_vs_wavelength = extract_reflection(node.child("set_surface"));
	extract_Plane_props(x_width, y_width, node.child("set_plane"));

	assert_name_of_child_frame_is_not_in_use_yet(mother, frameFab.get_name());	

	Plane *new_plane;
	new_plane = new Plane;

	new_plane->set_name_pos_rot(
		frameFab.get_name(), 
		frameFab.get_position(), 
		frameFab.get_rotation()
	);
	new_plane->set_inner_color(color);
	new_plane->set_outer_color(color);
	new_plane->set_outer_reflection(reflection_vs_wavelength);
	new_plane->set_inner_reflection(reflection_vs_wavelength);
	
	new_plane->set_x_y_width(x_width, y_width);
	
	mother->set_mother_and_child(new_plane);
	return new_plane;
}
//------------------------------------------------------------------------------
Frame* WorldFactory::produceSphere(
	Frame* mother,const pugi::xml_node node
){
	FrameFactory frameFab(node);
	assert_child_exists(node, "set_surface");
	assert_child_exists(node, "set_sphere");

	std::string 			name;
	Vector3D 				position;
	Rotation3D 				rotation;
	const Color*		color;
	const Function::Func1D* reflection_vs_wavelength;
	double 					radius;

	color = extract_color(node.child("set_surface"));
	reflection_vs_wavelength = extract_reflection(node.child("set_surface"));
	extract_Sphere_props(radius,node.child("set_sphere"));

	assert_name_of_child_frame_is_not_in_use_yet(mother, frameFab.get_name());	

	Sphere *new_sphere;
	new_sphere = new Sphere;	

	new_sphere->set_name_pos_rot(
		frameFab.get_name(), 
		frameFab.get_position(), 
		frameFab.get_rotation()
	);
	new_sphere->set_inner_color(color);
	new_sphere->set_outer_color(color);
	new_sphere->set_outer_reflection(reflection_vs_wavelength);
	new_sphere->set_inner_reflection(reflection_vs_wavelength);

	new_sphere->set_sphere_radius(radius);
	
	mother->set_mother_and_child(new_sphere);
	return new_sphere;
}
//------------------------------------------------------------------------------
Frame* WorldFactory::produceCylinder(
	Frame* mother,const pugi::xml_node node
){
	FrameFactory frameFab(node);
	assert_child_exists(node, "set_surface");
	assert_child_exists(node, "set_cylinder");

	std::string 			name;
	Vector3D 				position;
	Rotation3D 				rotation;
	const Color*			color;
	const Function::Func1D* reflection_vs_wavelength;
	double 					radius;
	Vector3D 				start, end;
					
	color = extract_color(node.child("set_surface"));
	reflection_vs_wavelength = extract_reflection(node.child("set_surface"));
	extract_Cylinder_props(radius, start, end, node.child("set_cylinder"));

	assert_name_of_child_frame_is_not_in_use_yet(mother, frameFab.get_name());	

	Cylinder *new_Cylinder;
	new_Cylinder = new Cylinder;
	new_Cylinder->set_name_pos_rot(
		frameFab.get_name(), 
		frameFab.get_position(), 
		frameFab.get_rotation()
	);
	new_Cylinder->set_inner_color(color);
	new_Cylinder->set_outer_color(color);
	new_Cylinder->set_outer_reflection(reflection_vs_wavelength);
	new_Cylinder->set_inner_reflection(reflection_vs_wavelength);	

	new_Cylinder->set_cylinder(radius, start, end);
	
	mother->set_mother_and_child(new_Cylinder);
	return new_Cylinder;
}
//------------------------------------------------------------------------------
Frame* WorldFactory::produceReflector(
	Frame* mother, const pugi::xml_node node
){
	FrameFactory frameFab(node);
	assert_child_exists(node, "set_segmented_reflector");
	const pugi::xml_node refl_node = node.child("set_segmented_reflector");

	std::string 		name;
	Vector3D 			position;
	Rotation3D 			rotation;
	const Function::Func1D* reflection_vs_wavelength;
					
	assert_name_of_child_frame_is_not_in_use_yet(mother, frameFab.get_name());
	reflection_vs_wavelength = extract_reflection(node.child("set_surface"));

	SegmentedReflector::GeometryCard geom_card;
	geom_card.focal_length = 
		extract_reflector("focal_length", refl_node);

	geom_card.DaviesCotton_over_parabolic_mixing_factor = 
		extract_reflector("DaviesCotton_over_parabolic_mixing_factor", refl_node);

	geom_card.max_outer_aperture_radius = 
		extract_reflector("max_outer_aperture_radius", refl_node);

	geom_card.min_inner_aperture_radius = 
		extract_reflector("min_inner_aperture_radius", refl_node);

	geom_card.facet_inner_hex_radius = 
		extract_reflector("facet_inner_hex_radius", refl_node);

	geom_card.gap_between_facets = 
		extract_reflector("gap_between_facets", refl_node);

	SegmentedReflector::SurfaceCard surf_card;
	surf_card.outer_mirror_reflection = reflection_vs_wavelength;

	SegmentedReflector::Factory refl_fab(geom_card, surf_card);
	Frame* reflector = refl_fab.get_reflector();

	mother->set_mother_and_child(reflector);
	return reflector;
}
//------------------------------------------------------------------------------
Frame* WorldFactory::produceBiConvexLensHex(
	Frame* mother, const pugi::xml_node node
) {
	FrameFactory frameFab(node);	
	assert_child_exists(node, "set_bi_convex_lens_hex");

	std::string 		name;
	Vector3D 			position;
	Rotation3D 			rotation;
	const Color*		color;
	const Function::Func1D* reflection_vs_wavelength;
	double curv_radius;
	double outer_radius;
	const Function::Func1D* refraction;
	//RefractiveIndex* lens_refractive_index;

	color = extract_color(node.child("set_surface"));
	assert_name_of_child_frame_is_not_in_use_yet(mother, frameFab.get_name());	
	extractBiConvexLensHex(curv_radius, outer_radius, node.child("set_bi_convex_lens_hex"));
	refraction = extract_refraction(node.child("set_medium"));
	reflection_vs_wavelength = extract_reflection(node.child("set_surface"));

	BiConvexLensHexBound *lens;
	lens = new BiConvexLensHexBound;
	lens->set_name_pos_rot(
		frameFab.get_name(), 
		frameFab.get_position(), 
		frameFab.get_rotation()
	);
	lens->set_inner_color(color);
	lens->set_outer_color(color);
	lens->set_outer_reflection(reflection_vs_wavelength);
	lens->set_inner_refraction(refraction);
	lens->set_curvature_radius_and_outer_hex_radius(curv_radius, outer_radius);
	
	mother->set_mother_and_child(lens);
	return lens;
}
//------------------------------------------------------------------------------
Frame* WorldFactory::produce_stl_object(
		Frame* mother, const pugi::xml_node node
) {
	FrameFactory frameFab(node);
	assert_name_of_child_frame_is_not_in_use_yet(mother, frameFab.get_name());
	
	assert_child_exists(node, "set_stl");
	const pugi::xml_node set_stl_node = node.child("set_stl");

	assert_attribute_exists(set_stl_node, "file");
	std::string file = 
		absolute_path + set_stl_node.attribute("file").value();

	assert_attribute_exists(set_stl_node, "scale");
	double scale = StrToDouble(set_stl_node.attribute("scale").value());

	Frame* object = StereoLitographyIo::read(file, scale);

	Frame* repositioned_object = new Frame(
		frameFab.get_name(), 
		frameFab.get_position(), 
		frameFab.get_rotation()
	);

	repositioned_object->take_children_from(object);
	
	//delete object;

	mother->set_mother_and_child(repositioned_object);
	return repositioned_object;
}
//------------------------------------------------------------------------------
const Function::Func1D* WorldFactory::extract_refraction(
	const pugi::xml_node node
) {
	if(has_attribute(node, "refraction_vs_wavelength")) {
		return functions->get_function(node.attribute("refraction_vs_wavelength").value());
	}else{
		std::stringstream info;
		info << "WorldFactory::" <<__func__<< "() ";
		info << "in " <<__FILE__<< ", " <<__LINE__<< "\n";
		throw MissingItem(info.str(), this, "refraction_vs_wavelength");
	}
}
//------------------------------------------------------------------------------
void WorldFactory::extractBiConvexLensHex(
	double &curv_radius, double &outer_radius, const pugi::xml_node node
) {
	assert_attribute_exists(node, "curvature_radius");
	assert_attribute_exists(node, "outer_radius");


	curv_radius = StrToDouble(node.attribute("curvature_radius").value());
	outer_radius = StrToDouble(node.attribute("outer_radius").value());
}
//------------------------------------------------------------------------------
double WorldFactory::extract_reflector(
	const std::string key, 
	const pugi::xml_node node
) {
	assert_attribute_exists(node, key.c_str());
	return StrToDouble(node.attribute(key.c_str()).value());
}
//------------------------------------------------------------------------------
Frame* WorldFactory::produceDisc(
	Frame* mother,const pugi::xml_node node
){
	FrameFactory frameFab(node);
	assert_child_exists(node, "set_surface");
	assert_child_exists(node, "set_disc");

	std::string 			name;
	Vector3D 				position;
	Rotation3D 				rotation;
	const Color*		color;
	const Function::Func1D* reflection_vs_wavelength;
	double 					radius;

	color = extract_color(node.child("set_surface"));
	reflection_vs_wavelength = extract_reflection(node.child("set_surface"));
	extract_Disc_props(radius, node.child("set_disc"));
	assert_name_of_child_frame_is_not_in_use_yet(mother, frameFab.get_name());	
	
	Disc *new_Disc;
	new_Disc = new Disc;	
	
	new_Disc->set_name_pos_rot(
		frameFab.get_name(), 
		frameFab.get_position(), 
		frameFab.get_rotation()
	);
	new_Disc->set_inner_color(color);
	new_Disc->set_outer_color(color);
	new_Disc->set_outer_reflection(reflection_vs_wavelength);
	new_Disc->set_inner_reflection(reflection_vs_wavelength);	

	new_Disc->set_disc_radius(radius);
	
	mother->set_mother_and_child(new_Disc);
	return new_Disc;
}
//------------------------------------------------------------------------------
Frame* WorldFactory::produceTriangle(
	Frame* mother,const pugi::xml_node node
){
	FrameFactory frameFab(node);
	assert_child_exists(node, "set_surface");
	assert_child_exists(node, "set_triangle");
	
	std::string 			name;
	Vector3D 				position;
	Rotation3D 				rotation;
	const Color*		color;
	const Function::Func1D* reflection_vs_wavelength;
	double 					Ax, Ay, Bx, By, Cx, Cy;

	color = extract_color(node.child("set_surface"));
	reflection_vs_wavelength = extract_reflection(node.child("set_surface"));
	extract_Triangle_props(Ax, Ay, Bx, By, Cx, Cy, node.child("set_triangle"));
	assert_name_of_child_frame_is_not_in_use_yet(mother, frameFab.get_name());

	Triangle *new_Triangle;
	new_Triangle = new Triangle;			
	new_Triangle->set_name_pos_rot(
		frameFab.get_name(), 
		frameFab.get_position(), 
		frameFab.get_rotation()
	);
	new_Triangle->set_inner_color(color);
	new_Triangle->set_outer_color(color);
	new_Triangle->set_outer_reflection(reflection_vs_wavelength);
	new_Triangle->set_inner_reflection(reflection_vs_wavelength);

	new_Triangle->set_corners_in_xy_plane(Ax, Ay, Bx, By, Cx, Cy);
	
	mother->set_mother_and_child(new_Triangle);
	return new_Triangle;
}
//------------------------------------------------------------------------------
Frame* WorldFactory::produce_sphere_cap_hexagonal(
	Frame* mother, const pugi::xml_node node
) {
	FrameFactory frameFab(node);
	assert_child_exists(node, "set_surface");
	assert_child_exists(node, "set_sphere_cap_hexagonal");

	std::string 			name;
	Vector3D 				position;
	Rotation3D 				rotation;
	const Color*		color;
	const Function::Func1D* reflection_vs_wavelength;

	color = extract_color(node.child("set_surface"));
	reflection_vs_wavelength = extract_reflection(node.child("set_surface"));

	SphereCapWithHexagonalBound* cap = new SphereCapWithHexagonalBound;
	double focal_length, outer_radius;
	
	extract_sphere_cap_hexagonal(
		focal_length, outer_radius, node.child("set_sphere_cap_hexagonal")
	);
	assert_name_of_child_frame_is_not_in_use_yet(mother, frameFab.get_name());

	cap->set_name_pos_rot(
		frameFab.get_name(), 
		frameFab.get_position(), 
		frameFab.get_rotation()
	);
	cap->set_inner_color(color);
	cap->set_outer_color(color);
	cap->set_outer_reflection(reflection_vs_wavelength);
	cap->set_inner_reflection(reflection_vs_wavelength);
	cap->set_curvature_radius_and_outer_hex_radius(2.0*focal_length, outer_radius);
	assert_name_of_child_frame_is_not_in_use_yet(mother, name);

	mother->set_mother_and_child(cap);
	return cap;
}
//------------------------------------------------------------------------------
void WorldFactory::extract_sphere_cap_hexagonal(
	double &focal_length, double &outer_radius, const pugi::xml_node node
) {
	assert_attribute_exists(node, "focal_length");
	assert_attribute_exists(node, "outer_radius");

	focal_length = StrToDouble(node.attribute("focal_length").value());
	outer_radius = StrToDouble(node.attribute("outer_radius").value());
}
//------------------------------------------------------------------------------
const Function::Func1D* WorldFactory::extract_reflection(
	const pugi::xml_node node
) {
	if(has_attribute(node, "reflection_vs_wavelength")) {
		return functions->get_function(node.attribute("reflection_vs_wavelength").value());
	}else{
		std::stringstream info;
		info << "WorldFactory::" <<__func__<< "() ";
		info << "in " <<__FILE__<< ", " <<__LINE__<< "\n";
		throw MissingItem(info.str(), this, "reflection_vs_wavelength");
	}
}
//------------------------------------------------------------------------------
const Color* WorldFactory::extract_color(const pugi::xml_node node) {
	assert_attribute_exists(node, "colour");

	double red, blu, gre;
	strto3tuple(red, blu, gre, node.attribute("colour").value());
	
	Color* colour = new Color(red, blu, gre);
	return colour;
}
//------------------------------------------------------------------------------
void WorldFactory::extract_Plane_props(
	double &x_width, double &y_width, const pugi::xml_node node 
){
	assert_attribute_exists(node, "x_width");
	assert_attribute_exists(node, "y_width");
	
	x_width = StrToDouble(node.attribute("x_width").value());
	y_width = StrToDouble(node.attribute("y_width").value());
}
//------------------------------------------------------------------------------
void WorldFactory::extract_Sphere_props(
	double &radius,const pugi::xml_node node
){
	assert_attribute_exists(node, "radius");	
	
	radius = StrToDouble(node.attribute("radius").value());
}
//------------------------------------------------------------------------------
void WorldFactory::extract_Cylinder_props(
	double &radius,
	Vector3D &start,
	Vector3D &end,
	const pugi::xml_node node
){
	assert_attribute_exists(node, "radius");
	assert_attribute_exists(node, "start_pos");
	assert_attribute_exists(node, "end_pos");
		
	radius = StrToDouble(node.attribute("radius").value());
	
	double sX, sY, sZ;
	strto3tuple(sX, sY, sZ, node.attribute("start_pos").value());
	start.set(sX, sY, sZ);
	
	double eX, eY, eZ;
	strto3tuple(eX, eY, eZ, node.attribute("end_pos").value());
	end.set(eX, eY, eZ);
}
//------------------------------------------------------------------------------
void WorldFactory::extract_Disc_props(double &radius,const pugi::xml_node node){

	assert_attribute_exists(node, "radius");

	radius = StrToDouble(node.attribute("radius").value());
}
//------------------------------------------------------------------------------
void WorldFactory::extract_Triangle_props(
	double &Ax, double &Ay, 
	double &Bx, double &By, 
	double &Cx, double &Cy,
	const pugi::xml_node node
){
	assert_attribute_exists(node, "Ax");
	assert_attribute_exists(node, "Ay");
	assert_attribute_exists(node, "Bx");
	assert_attribute_exists(node, "By");
	assert_attribute_exists(node, "Cx");
	assert_attribute_exists(node, "Cy");
 	
 	Ax = StrToDouble(node.attribute("Ax").value());
  	Ay = StrToDouble(node.attribute("Ay").value());
   	Bx = StrToDouble(node.attribute("Bx").value());
 	By = StrToDouble(node.attribute("By").value());
    Cx = StrToDouble(node.attribute("Cx").value());
 	Cy = StrToDouble(node.attribute("Cy").value());
}
//------------------------------------------------------------------------------
void WorldFactory::assert_name_of_child_frame_is_not_in_use_yet(
	const Frame *mother,
	const std::string name_of_additional_child
)const{

	if( mother->has_child_with_name(name_of_additional_child) ){
		// There is already a child in the mother frame with this name.
		// There must not be a second one!
		std::stringstream info;
		info << "WorldFactory::" << __func__ << "()";
		throw MultipleUseage(
			info.str(), this, 
			mother->
				get_child_by_name(name_of_additional_child)->
					get_path_in_tree_of_frames()
		);
	}
}
//------------------------------------------------------------------------------
Frame* WorldFactory::world(){
	root_of_World->init_tree_based_on_mother_child_relations();
	return root_of_World;
}
//------------------------------------------------------------------------------
std::vector<PhotonSensor::Sensor*>* WorldFactory::sensors_in_world()const {
	PhotonSensors::sort_photon_sensors_based_on_frames(sensors);
	return sensors;
}
//------------------------------------------------------------------------------
TelescopeArrayControl* WorldFactory::get_telescope_array_control()const {
	return telescopes;
} 