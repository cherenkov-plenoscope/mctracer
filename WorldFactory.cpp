#include "WorldFactory.h"
//------------------------------------------------------------------------------
WorldFactory::WorldFactory(){

	root_of_World = new Frame(
		"world",
		Vector3D::null,
		Rotation3D::null
	);
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

	//try{
    	load_file(root_of_World, absolute_path, filename);
    /*}catch(TracerException &error) {
        std::stringstream info;
		info << "WorldFactory::" << __func__ << "()\n";
		info << StringTools::place_first_infront_of_each_new_line_of_second(
		   			"| ", 
		   			error.what() 
		   		);
		throw XmlIoException(info.str(), this);	
    }*/
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
void WorldFactory::fabricate_frame(
Frame* mother,const pugi::xml_node node){
	
	XmlNode = node;

	if(		 StringTools::is_equal(node.name(),"frame")){
		mother = produceCartesianFrame(mother,node);

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
	
	}else if(StringTools::is_equal(node.name(),"reflector")){
		mother = produceReflector(mother,node);	

	}else if(StringTools::is_equal(node.name(),"include")){
		include_file(mother,node);		
		
	}else if( mother->has_mother() ){	
		std::stringstream info;
		info << "WorldFactory::" << __func__ << "() found an unknown item.";
		throw UnknownItem( info.str(), this ,node.name());
	}
	
	go_on_with_children_of_node(mother,node);
}
//------------------------------------------------------------------------------
void WorldFactory::go_on_with_children_of_node(
	Frame* mother,const pugi::xml_node node){
	
	// go on with children of node
	for(
		pugi::xml_node sub_node = node.first_child(); 
		sub_node; 
		sub_node = sub_node.next_sibling()
	){	
		std::string sub_node_name = sub_node.name();

		if 		(StringTools::is_equal(sub_node_name,"include")){

			fabricate_frame(mother,sub_node);
		}else if(StringTools::is_equal(sub_node_name,"frame")){

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
		}else if(StringTools::is_equal(sub_node_name,"reflector")){

			fabricate_frame(mother,sub_node);
		}else if(sub_node_name.find("set") == std::string::npos){

			std::stringstream info;
			info << "WorldFactory::" << __func__ << "() found an unknown item.";
			throw UnknownItem(info.str(), this, sub_node_name);
		}	
	}	
}
//------------------------------------------------------------------------------
Frame* WorldFactory::produceCartesianFrame(
	Frame* mother,
	const pugi::xml_node node
){
	assert_child_exists(node, "set_frame");

	std::string 			name;
	Vector3D 				position;
	Rotation3D 				rotation;
	
	extract_Frame_props(name, position, rotation, node.child("set_frame") );
		
	Frame *frame;
	frame = new Frame(name,position,rotation);

	mother->set_mother_and_child(frame);
	return frame;
}
//------------------------------------------------------------------------------
Frame* WorldFactory::producePlane(
	Frame* mother,const pugi::xml_node node
){
	assert_child_exists(node, "set_frame");
	assert_child_exists(node, "set_surface");
	assert_child_exists(node, "set_plane");

	std::string 			name;
	Vector3D    			position;
	Rotation3D  			rotation;
	const Color*		color;
	const ReflectionProperties*	refl_prop;
	double 					x_width, y_width;
	
	extract_Frame_props(name, position, rotation, node.child("set_frame"));
	color = extract_color(node.child("set_surface"));
	refl_prop = extract_reflection(node.child("set_surface"));
	extract_Plane_props(x_width, y_width, node.child("set_plane"));

	assert_name_of_child_frame_is_not_in_use_yet(mother, name);	

	Plane *new_plane;
	new_plane = new Plane;

	new_plane->set_name_pos_rot(name,position,rotation);
	new_plane->set_inner_color(color);
	new_plane->set_outer_color(color);
	new_plane->set_outer_reflection(refl_prop);
	new_plane->set_inner_reflection(refl_prop);
	
	new_plane->set_plane_using_x_and_y_width(x_width, y_width);
	
	mother->set_mother_and_child(new_plane);
	return new_plane;
}
//------------------------------------------------------------------------------
Frame* WorldFactory::produceSphere(
	Frame* mother,const pugi::xml_node node
){
	assert_child_exists(node, "set_frame");
	assert_child_exists(node, "set_surface");
	assert_child_exists(node, "set_sphere");

	std::string 			name;
	Vector3D 				position;
	Rotation3D 				rotation;
	const Color*		color;
	const ReflectionProperties*	refl_prop;
	double 					radius;

	extract_Frame_props(name, position, rotation, node.child("set_frame"));
	color = extract_color(node.child("set_surface"));
	refl_prop = extract_reflection(node.child("set_surface"));
	extract_Sphere_props(radius,node.child("set_sphere"));

	assert_name_of_child_frame_is_not_in_use_yet(mother, name);

	Sphere *new_sphere;
	new_sphere = new Sphere;	

	new_sphere->set_name_pos_rot(name,position,rotation);
	new_sphere->set_inner_color(color);
	new_sphere->set_outer_color(color);
	new_sphere->set_outer_reflection(refl_prop);
	new_sphere->set_inner_reflection(refl_prop);

	new_sphere->set_sphere_radius(radius);
	
	mother->set_mother_and_child(new_sphere);
	return new_sphere;
}
//------------------------------------------------------------------------------
Frame* WorldFactory::produceCylinder(
	Frame* mother,const pugi::xml_node node
){
	assert_child_exists(node, "set_frame");
	assert_child_exists(node, "set_surface");
	assert_child_exists(node, "set_cylinder");

	std::string 			name;
	Vector3D 				position;
	Rotation3D 				rotation;
	const Color*		color;
	const ReflectionProperties*	refl_prop;
	double 					radius;
	Vector3D 				start, end;
					
	extract_Frame_props(name,position, rotation, node.child("set_frame"));
	color = extract_color(node.child("set_surface"));
	refl_prop = extract_reflection(node.child("set_surface"));
	extract_Cylinder_props(radius, start, end, node.child("set_cylinder"));

	assert_name_of_child_frame_is_not_in_use_yet(mother, name);

	Cylinder *new_Cylinder;
	new_Cylinder = new Cylinder;
	new_Cylinder->set_name_pos_rot(name,position, rotation);
	new_Cylinder->set_inner_color(color);
	new_Cylinder->set_outer_color(color);
	new_Cylinder->set_outer_reflection(refl_prop);
	new_Cylinder->set_inner_reflection(refl_prop);	

	new_Cylinder->set_cylinder(radius, start, end);
	
	mother->set_mother_and_child(new_Cylinder);
	return new_Cylinder;
}
//------------------------------------------------------------------------------
Frame* WorldFactory::produceReflector(
	Frame* mother, const pugi::xml_node node
){
	assert_child_exists(node, "set_frame");	
	assert_child_exists(node, "set_reflector");
	const pugi::xml_node refl_node = node.child("set_reflector");


	std::string 		name;
	Vector3D 			position;
	Rotation3D 			rotation;
					
	extract_Frame_props(name, position, rotation, node.child("set_frame"));
	assert_name_of_child_frame_is_not_in_use_yet(mother, name);

	SegmetedReflectorGenerator refl_gen;
	refl_gen.set_focal_length(extract_reflector("focal_length", refl_node));
	refl_gen.set_facet_spacing(extract_reflector("facet_spacing", refl_node));
	refl_gen.set_max_outer_diameter(extract_reflector("max_outer_diameter", refl_node));
	refl_gen.set_min_inner_diameter(extract_reflector("min_inner_diameter", refl_node));
	refl_gen.set_hybrid_geometry(extract_reflector("alpha", refl_node));
	Frame* reflector = refl_gen.get_reflector();

	mother->set_mother_and_child(reflector);
	return reflector;
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
	assert_child_exists(node, "set_frame");	
	assert_child_exists(node, "set_surface");
	assert_child_exists(node, "set_disc");

	std::string 			name;
	Vector3D 				position;
	Rotation3D 				rotation;
	const Color*		color;
	const ReflectionProperties*	refl_prop;
	double 					radius;

	extract_Frame_props(name, position, rotation, node.child("set_frame"));
	color = extract_color(node.child("set_surface"));
	refl_prop = extract_reflection(node.child("set_surface"));
	extract_Disc_props(radius, node.child("set_disc"));

	assert_name_of_child_frame_is_not_in_use_yet(mother, name);
	
	Disc *new_Disc;
	new_Disc = new Disc;	
	
	new_Disc->set_name_pos_rot(name, position, rotation);
	new_Disc->set_inner_color(color);
	new_Disc->set_outer_color(color);
	new_Disc->set_outer_reflection(refl_prop);
	new_Disc->set_inner_reflection(refl_prop);	

	new_Disc->set_disc_radius(radius);
	
	mother->set_mother_and_child(new_Disc);
	return new_Disc;
}
//------------------------------------------------------------------------------
Frame* WorldFactory::produceTriangle(
	Frame* mother,const pugi::xml_node node
){
	assert_child_exists(node, "set_frame");
	assert_child_exists(node, "set_surface");
	assert_child_exists(node, "set_triangle");
	
	std::string 			name;
	Vector3D 				position;
	Rotation3D 				rotation;
	const Color*		color;
	const ReflectionProperties*	refl_prop;
	double 					Ax, Ay, Bx, By, Cx, Cy;

	extract_Frame_props(name, position, rotation, node.child("set_frame"));
	color = extract_color(node.child("set_surface"));
	refl_prop = extract_reflection(node.child("set_surface"));
	extract_Triangle_props(Ax, Ay, Bx, By, Cx, Cy, node.child("set_triangle"));

	assert_name_of_child_frame_is_not_in_use_yet(mother, name);

	Triangle *new_Triangle;
	new_Triangle = new Triangle;			
	new_Triangle->set_name_pos_rot(name,position,rotation);
	new_Triangle->set_inner_color(color);
	new_Triangle->set_outer_color(color);
	new_Triangle->set_outer_reflection(refl_prop);
	new_Triangle->set_inner_reflection(refl_prop);

	new_Triangle->set_corners_in_xy_plane(Ax, Ay, Bx, By, Cx, Cy);
	
	mother->set_mother_and_child(new_Triangle);
	return new_Triangle;
}
//------------------------------------------------------------------------------
const ReflectionProperties* WorldFactory::extract_reflection(
	const pugi::xml_node node
) {
	assert_attribute_exists(node, "refl");

	std::string refl_attribure = node.attribute("refl").value();

	// In case the reflection attribute string ends with '.xml', then the
	// reflection coefficient is parsed in out of a xml file. 
	// There the reflection coefficient can be defined as a function of the
	// wavelength. 

	ReflectionProperties* refl_prop;
	if( StringTools::is_ending(refl_attribure, ".xml") )
		refl_prop = new ReflectionProperties((absolute_path + refl_attribure));
	else
		refl_prop = new ReflectionProperties(StrToDouble(refl_attribure));

	return refl_prop;
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
void WorldFactory::extract_Frame_props(
	std::string &name, Vector3D &position, Rotation3D &rotation,
	const pugi::xml_node node
){	
	assert_attribute_exists(node, "name");
	assert_attribute_exists(node, "pos");
	assert_attribute_exists(node, "rot");	

	name = node.attribute("name").value();

	double pX, pY, pZ;
	strto3tuple(pX, pY, pZ, node.attribute("pos").value());
	position.set(pX, pY, pZ);

	double rX, rY, rZ;
	strto3tuple(rX, rY, rZ, node.attribute("rot").value());
	rotation.set(rX, rY, rZ);
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
		// There is already a child in the mother frame wit this name.
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