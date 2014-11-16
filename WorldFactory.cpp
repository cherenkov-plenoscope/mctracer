#include "WorldFactory.h"
//------------------------------------------------------------------------------
WorldFactory::WorldFactory(){

	root_of_World = new CartesianFrame;
	root_of_World->set_frame(
		"world",
		Vector3D(0.0,0.0,0.0),
		Rotation3D(0.0,0.0,0.0)
	);
}
//------------------------------------------------------------------------------
void WorldFactory::load(std::string path){

	// determine directory
	int position_in_path = path.find_last_of("/\\");

	if ( position_in_path == -1 )	{
		absolute_path = "";
	}else{
		absolute_path = path.substr(0,position_in_path + 1); 
	}

	std::string filename = path.substr(position_in_path + 1); 

    load_file(root_of_World, absolute_path, filename);
}
//------------------------------------------------------------------------------
void WorldFactory::load_file(
	CartesianFrame* mother,
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
		info << "WorldFactory::"<<__func__<<"()";
		throw XmlIoException(info.str(), this);
	}
}
//------------------------------------------------------------------------------
void WorldFactory::include_file(
	CartesianFrame* mother, const pugi::xml_node node
){
	std::string relative_path_to_xml_to_be_included;
	extract_include_path(relative_path_to_xml_to_be_included, node);

	WorldFactory fab;
	fab.load(absolute_path + relative_path_to_xml_to_be_included);

	CartesianFrame *sub_world;
	sub_world = fab.world();

	mother->take_children(sub_world);
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
CartesianFrame* mother,const pugi::xml_node node){
	
	XmlNode = node;

	if(		 pedantic_str_comp(node.name(),"frame")){
		mother = produceCartesianFrame(mother,node);

	}else if(pedantic_str_comp(node.name(),"triangle")){
		mother = produceTriangle(mother,node);
		
	}else if(pedantic_str_comp(node.name(),"plane")){
		mother = producePlane(mother,node);
		
	}else if(pedantic_str_comp(node.name(),"sphere")){
		mother = produceSphere(mother,node);	

	}else if(pedantic_str_comp(node.name(),"cylinder")){
		mother = produceCylinder(mother,node);	

	}else if(pedantic_str_comp(node.name(),"disc")){
		mother = produceDisc(mother,node);	
	
	}else if(pedantic_str_comp(node.name(),"FACT_reflector")){
		mother = produceFactReflector(mother,node);	

	}else if(pedantic_str_comp(node.name(),"include")){
		include_file(mother,node);		
		
	}else if( mother->has_mother() ){	
		std::stringstream info;
		info << "WorldFactory::"<<__func__<<"() found an unknown item.";
		throw UnknownItem( info.str(), this ,node.name());
	}
	
	go_on_with_children_of_node(mother,node);
}
//------------------------------------------------------------------------------
void WorldFactory::go_on_with_children_of_node(
	CartesianFrame* mother,const pugi::xml_node node){
	
	// go on with children of node
	for(
		pugi::xml_node sub_node = node.first_child(); 
		sub_node; 
		sub_node = sub_node.next_sibling()
	){	
		
		std::string sub_node_name = sub_node.name();
		bool valid_child = false;

		if(pedantic_str_comp(sub_node_name,"include")){
			fabricate_frame(mother,sub_node); valid_child=true;
		}				
		if(pedantic_str_comp(sub_node_name,"frame")){
			fabricate_frame(mother,sub_node); valid_child=true;
		}
		if(pedantic_str_comp(sub_node_name,"triangle")){
			fabricate_frame(mother,sub_node); valid_child=true;
		}	
		if(pedantic_str_comp(sub_node_name,"plane")){
			fabricate_frame(mother,sub_node); valid_child=true;
		}
		if(pedantic_str_comp(sub_node_name,"sphere")){
			fabricate_frame(mother,sub_node); valid_child=true;
		}
		if(pedantic_str_comp(sub_node_name,"cylinder")){
			fabricate_frame(mother,sub_node); valid_child=true;
		}	
		if(pedantic_str_comp(sub_node_name,"disc")){
			fabricate_frame(mother,sub_node); valid_child=true;
		}	
		if(pedantic_str_comp(sub_node_name,"FACT_reflector")){
			fabricate_frame(mother,sub_node); valid_child=true;
		}	
		
		if(!valid_child){
			if(sub_node_name.find("set") == std::string::npos){

				std::stringstream info;
				info << "WorldFactory::"<<__func__<<"() found an unknown item.";
				throw UnknownItem(info.str(), this, sub_node_name);
			}
		}
	}	
}
//------------------------------------------------------------------------------
CartesianFrame* WorldFactory::produceCartesianFrame(
	CartesianFrame* mother,
	const pugi::xml_node node
){
	assert_child_exists(node, "set_frame");

	std::string 			name;
	Vector3D 				position;
	Rotation3D 				rotation;
	
	extract_Frame_props(name, position, rotation, node.child("set_frame") );
		
	CartesianFrame *frame;
	frame = new CartesianFrame;
	
	frame->set_frame(name,position,rotation);

	mother->set_mother_and_child(frame);
	return frame;
}
//------------------------------------------------------------------------------
CartesianFrame* WorldFactory::producePlane(
	CartesianFrame* mother,const pugi::xml_node node
){
	assert_child_exists(node, "set_frame");
	assert_child_exists(node, "set_surface");
	assert_child_exists(node, "set_plane");

	std::string 			name;
	Vector3D    			position;
	Rotation3D  			rotation;
	ReflectionProperties 	refl_prop; 
	ColourProperties 		colour;
	double 					min_x, max_x, min_y, max_y;
	
	extract_Frame_props(name, position, rotation, node.child("set_frame"));
	extract_Surface_props(refl_prop, colour, node.child("set_surface"));
	extract_Plane_props(min_x, max_x, min_y, max_y, node.child("set_plane"));

	assert_name_of_child_frame_is_not_in_use_yet(mother, name);	

	Plane *new_plane;
	new_plane = new Plane;

	new_plane->set_frame(name,position,rotation);
	new_plane->set_surface_properties(&refl_prop,&colour);
	new_plane->set_plane(min_x, max_x, min_y, max_y);
	
	mother->set_mother_and_child(new_plane);
	return new_plane;
}
//------------------------------------------------------------------------------
CartesianFrame* WorldFactory::produceSphere(
	CartesianFrame* mother,const pugi::xml_node node
){
	assert_child_exists(node, "set_frame");
	assert_child_exists(node, "set_surface");
	assert_child_exists(node, "set_sphere");

	std::string 			name;
	Vector3D 				position;
	Rotation3D 				rotation;
	ReflectionProperties 	refl_prop; 
	ColourProperties 		colour;
	double 					radius;

	extract_Frame_props(name, position, rotation, node.child("set_frame"));
	extract_Surface_props(refl_prop, colour, node.child("set_surface"));
	extract_Sphere_props(radius,node.child("set_sphere"));

	assert_name_of_child_frame_is_not_in_use_yet(mother, name);

	Sphere *new_sphere;
	new_sphere = new Sphere;	

	new_sphere->set_frame(name,position,rotation);
	new_sphere->set_surface_properties(&refl_prop,&colour);
	new_sphere->set_sphere(radius);
	
	mother->set_mother_and_child(new_sphere);
	return new_sphere;
}
//------------------------------------------------------------------------------
CartesianFrame* WorldFactory::produceCylinder(
	CartesianFrame* mother,const pugi::xml_node node
){
	assert_child_exists(node, "set_frame");
	assert_child_exists(node, "set_surface");
	assert_child_exists(node, "set_cylinder");

	std::string 			name;
	Vector3D 				position;
	Rotation3D 				rotation;
	ReflectionProperties 	refl_prop; 
	ColourProperties 		colour;
	double 					radius;
	Vector3D 				start, end;
					
	extract_Frame_props(name,position, rotation, node.child("set_frame"));
	extract_Surface_props(refl_prop, colour, node.child("set_surface"));
	extract_Cylinder_props(radius, start, end, node.child("set_cylinder"));

	assert_name_of_child_frame_is_not_in_use_yet(mother, name);

	Cylinder *new_Cylinder;
	new_Cylinder = new Cylinder;
	new_Cylinder->set_frame(name,position, rotation);
	new_Cylinder->set_surface_properties(&refl_prop, &colour);
	new_Cylinder->set_cylinder(radius, start, end);
	
	mother->set_mother_and_child(new_Cylinder);
	return new_Cylinder;
}
//------------------------------------------------------------------------------
CartesianFrame* WorldFactory::produceFactReflector(
	CartesianFrame* mother,const pugi::xml_node node
){
	assert_child_exists(node, "set_frame");	
	assert_child_exists(node, "set_FACT_reflector");
	
	std::string 		name;
	Vector3D 			position;
	Rotation3D 			rotation;
	double 				alpha;
					
	extract_Frame_props(name, position, rotation, node.child("set_frame"));
	extract_FACT_props(alpha, node.child("set_FACT_reflector"));

	assert_name_of_child_frame_is_not_in_use_yet(mother, name);

	FactTelescope *new_FactTelescope;
	new_FactTelescope = new FactTelescope(alpha);			
	new_FactTelescope->set_frame(name,position,rotation);
	new_FactTelescope->init();
	
	mother->set_mother_and_child(new_FactTelescope);
	return new_FactTelescope;
}
//------------------------------------------------------------------------------
CartesianFrame* WorldFactory::produceDisc(
	CartesianFrame* mother,const pugi::xml_node node
){
	assert_child_exists(node, "set_frame");	
	assert_child_exists(node, "set_surface");
	assert_child_exists(node, "set_disc");

	std::string 			name;
	Vector3D 				position;
	Rotation3D 				rotation;
	ReflectionProperties 	refl_prop; 
	ColourProperties 		colour;
	double 					radius;

	extract_Frame_props(name, position, rotation, node.child("set_frame"));
	extract_Surface_props(refl_prop, colour, node.child("set_surface"));
	extract_Disc_props(radius, node.child("set_disc"));

	assert_name_of_child_frame_is_not_in_use_yet(mother, name);
	
	Disc *new_Disc;
	new_Disc = new Disc;	
	
	new_Disc->set_frame(name, position, rotation);
	new_Disc->set_surface_properties(&refl_prop, &colour);
	new_Disc->set_Disc(radius);
	
	mother->set_mother_and_child(new_Disc);
	return new_Disc;
}
//------------------------------------------------------------------------------
CartesianFrame* WorldFactory::produceTriangle(
	CartesianFrame* mother,const pugi::xml_node node
){
	assert_child_exists(node, "set_frame");
	assert_child_exists(node, "set_surface");
	assert_child_exists(node, "set_triangle");
	
	std::string 			name;
	Vector3D 				position;
	Rotation3D 				rotation;
	ReflectionProperties 	refl_prop; 
	ColourProperties 		colour;
	Vector3D 				pA, pB, pC;

	extract_Frame_props(name, position, rotation, node.child("set_frame"));
	extract_Surface_props(refl_prop, colour, node.child("set_surface"));
	extract_Triangle_props(pA, pB, pC, node.child("set_triangle"));

	assert_name_of_child_frame_is_not_in_use_yet(mother, name);

	Triangle *new_Triangle;
	new_Triangle = new Triangle;			
	new_Triangle->set_frame(name,position,rotation);
	new_Triangle->set_surface_properties(&refl_prop, &colour);
	new_Triangle->set_Triangle(pA, pB, pC);
	
	mother->set_mother_and_child(new_Triangle);
	return new_Triangle;
}
//------------------------------------------------------------------------------
void WorldFactory::extract_Surface_props(
	ReflectionProperties &refl_prop, 
	ColourProperties &colour,
	const pugi::xml_node node
){
	assert_attribute_exists(node, "refl");
	assert_attribute_exists(node, "colour");

	std::string refl_attribure = node.attribute("refl").value();

	// In case the reflection attribute string ends with '.xml', then the
	// reflection coefficient is parsed in out of a xml file. 
	// There the reflection coefficient can be defined as a function of the
	// wavelength. 
	if( is_ending(refl_attribure, ".xml") ){
		// set wavelength depending reflection coefficient
		refl_prop.SetReflectionCoefficient( (absolute_path + refl_attribure) );
	}else{
		// set reflection coefficient independent of wavelength	
		refl_prop.SetReflectionCoefficient(pedantic_strtod(refl_attribure));
	}	

	// color properties for camera rays to render images of the scenery
	//tuple3 col; 
	double red, blu, gre;
	strto3tuple(red, blu, gre, node.attribute("colour").value());
	colour.set_colour_0to255(red, blu, gre);
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
	double &min_x, double &max_x, double &min_y, double &max_y,
	const pugi::xml_node node
){
	assert_attribute_exists(node, "min_x");
	assert_attribute_exists(node, "min_x");
	assert_attribute_exists(node, "min_x");
	assert_attribute_exists(node, "min_x");
	
	min_x = pedantic_strtod(node.attribute("min_x").value());
	max_x = pedantic_strtod(node.attribute("max_x").value());
	min_y = pedantic_strtod(node.attribute("min_y").value());
	max_y = pedantic_strtod(node.attribute("max_y").value());
}
//------------------------------------------------------------------------------
void WorldFactory::extract_Sphere_props(
	double &radius,const pugi::xml_node node
){
	assert_attribute_exists(node, "radius");	
	
	radius = pedantic_strtod(node.attribute("radius").value());
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
		
	radius = pedantic_strtod(node.attribute("radius").value());
	
	double sX, sY, sZ;
	strto3tuple(sX, sY, sZ, node.attribute("start_pos").value());
	start.set(sX, sY, sZ);
	
	double eX, eY, eZ;
	strto3tuple(eX, eY, eZ, node.attribute("end_pos").value());
	end.set(eX, eY, eZ);
}
//------------------------------------------------------------------------------
void WorldFactory::extract_FACT_props(
	double &alpha,const pugi::xml_node node
){
	if(node.attribute("alpha") == NULL){
		
		std::stringstream info;
		info << "The set_FACT_reflector requires the 'alpha' statement! ";
		info << "Alpha=0 is a Davies-Cotton Reflector, alpha=1 is ";
		info << "a paraboloid reflector. Alpha in between 0 and 1 ";
		info << "is a mixture of both.";
		throw MissingItem(info.str(), this, "alpha" );
	}
	
	alpha = pedantic_strtod(node.attribute("alpha").value());
}
//------------------------------------------------------------------------------
void WorldFactory::extract_Disc_props(double &radius,const pugi::xml_node node){

	assert_attribute_exists(node, "radius");

	radius = pedantic_strtod(node.attribute("radius").value());
}
//------------------------------------------------------------------------------
void WorldFactory::extract_Triangle_props(
	Vector3D &point_A,
	Vector3D &point_B,
	Vector3D &point_C,
	const pugi::xml_node node
){
	assert_attribute_exists(node, "A");
	assert_attribute_exists(node, "B");
	assert_attribute_exists(node, "C");
 	
	double 		aX, aY, aZ;	
	strto3tuple(aX, aY, aZ, node.attribute("A").value());
	point_A.set(aX, aY, aZ);

	double 		bX, bY, bZ;	
	strto3tuple(bX, bY, bZ, node.attribute("B").value());
	point_B.set(bX, bY, bZ);

	double 		cX, cY, cZ;	
	strto3tuple(cX, cY, cZ, node.attribute("C").value());
	point_C.set(cX, cY, cZ);
}
//------------------------------------------------------------------------------
void WorldFactory::assert_name_of_child_frame_is_not_in_use_yet(
	const CartesianFrame *mother,
	const std::string name_of_additional_child
)const{

	if( mother->has_child_with_name(name_of_additional_child) ){
		// There is already a child in the mother frame wit this name.
		// There must not be a second one!
		stringstream info;
		info << "WorldFactory::"<<__func__<<"()";
		throw MultipleUseage(info.str(), this, 
			mother->
			get_pointer_to_specific_child(name_of_additional_child)->
			get_path()
		);
	}
}
//------------------------------------------------------------------------------
CartesianFrame* WorldFactory::world(){
	
	root_of_World->post_initialize_me_and_all_my_children();
	root_of_World->update_enclosing_sphere_for_all_children();
	return root_of_World;
}
//------------------------------------------------------------------------------