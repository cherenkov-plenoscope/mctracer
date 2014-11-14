#include "WorldFactory.h"
//------------------------------------------------------------------------------
WorldFactory::WorldFactory(){

	root_of_World = new CartesianFrame;
	root_of_World->set_frame(
		"world",
		Vector3D(0.0,0.0,0.0),
		Rotation3D(0.0,0.0,0.0)
	);

	verbose = false;

	absolute_path ="";
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

	if(verbose){
		std::cout << "path: " << path << "\n";
		std::cout << "directory ends at position: " << position_in_path << "\n";
		std::cout << "directory: " << absolute_path << "\n";
		std::cout << "filename : " << filename << "\n";
	}

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

    	if(verbose){
    		std::stringstream out;
        	out << "XML [" << path_of_file_to_load << "] ";
        	out << "parsing successful";
			cout << out.str() << endl;
		}

		frame_factory(mother,doc);	
    }else{
    	std::stringstream info;
		info << "WorldFactory::"<<__func__<<"()";
		throw XmlIoException(info.str(), this);
	}
}
//------------------------------------------------------------------------------
void WorldFactory::include_file(
	CartesianFrame* mother,const pugi::xml_node node){
	
	std::string relative_path;
	extract_path(relative_path,node);

	WorldFactory fab;
	fab.load(absolute_path + relative_path);

	CartesianFrame *sub_world;
	sub_world = fab.world();

	mother->take_children(sub_world);
}
//------------------------------------------------------------------------------
void WorldFactory::extract_path(std::string &path,const pugi::xml_node node){
	
	if(node.attribute("path") == NULL){

		std::stringstream info;
		info << "The 'include' statement needs the 'path' statement to define ";
		info << "the path to the xml file to be included!";
		throw XmlIoException(info.str(),this);
	}

	path = node.attribute("path").value();

	if(verbose)
		std::cout << "Include path = " << path << std::endl;	
}
//------------------------------------------------------------------------------
void WorldFactory::frame_factory(
CartesianFrame* mother,const pugi::xml_node node){
	
	if(verbose){
		std::stringstream out;
		out << "frame factory checking node: "<<node.name();
		out << " name=";
		out << node.child("set_frame").attribute("name").value();
		cout << out.str() << endl;
	}

	// set the cureent node working on
	XmlNode = node;

	//check node
	std::string node_name = node.name();
	if 		(node_name.compare("frame")==0){
		mother = produceCartesianFrame(mother,node);

	}else if(node_name.compare("triangle")==0){
		mother = produceTriangle(mother,node);
		
	}else if(node_name.compare("plane")==0){
		mother = producePlane(mother,node);
		
	}else if(node_name.compare("sphere")==0){
		mother = produceSphere(mother,node);	

	}else if(node_name.compare("cylinder")==0){
		mother = produceCylinder(mother,node);	

	}else if(node_name.compare("disc")==0){
		mother = produceDisc(mother,node);	
	
	}else if(node_name.compare("FACT_reflector")==0){
		mother = produceFactReflector(mother,node);	

	}else if(node_name.compare("include")==0){
		include_file(mother,node);	
		if(verbose){
			std::cout << "Found an include\n";
		}		
		
	}else{
		if(mother->get_number_of_children()==0){
			// Ignore the root of the world. 
			// Here no frame pos and rot must be set!
		}else{
			std::stringstream info;
			info << "WorldFactory::"<<__func__<<"() found an unknown item.";
			throw UnknownItem( info.str(), this ,node_name);
		}
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

		if(sub_node_name.compare("include")==0){
			frame_factory(mother,sub_node); valid_child=true;
		}				
		if(sub_node_name.compare("frame")==0){
			frame_factory(mother,sub_node); valid_child=true;
		}
		if(sub_node_name.compare("triangle")==0){
			frame_factory(mother,sub_node); valid_child=true;
		}	
		if(sub_node_name.compare("plane")==0){
			frame_factory(mother,sub_node); valid_child=true;
		}
		if(sub_node_name.compare("sphere")==0){
			frame_factory(mother,sub_node); valid_child=true;
		}
		if(sub_node_name.compare("cylinder")==0){
			frame_factory(mother,sub_node); valid_child=true;
		}	
		if(sub_node_name.compare("disc")==0){
			frame_factory(mother,sub_node); valid_child=true;
		}	
		if(sub_node_name.compare("FACT_reflector")==0){
			frame_factory(mother,sub_node); valid_child=true;
		}	
		if(valid_child){

			if(verbose){
				std::stringstream out;
				out << "frame_factory -> calling frame_factory for child >";
				out << sub_node.name() << "< of node >";
				out << node.child("set_frame").attribute("name").value() << "<";
				cout << out.str() << endl;	
			}
		}else{
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
	check(node, "set_frame");

	std::string 			name;
	Vector3D 				position;
	Rotation3D 				rotation;
	
	extract_Frame_props(name,position,rotation,node);
		
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
	check(node, "set_frame");
	check(node, "set_surface");
	check(node, "set_plane");

	std::string 			name;
	Vector3D    			position;
	Rotation3D  			rotation;
	ReflectionProperties 	refl_prop; 
	ColourProperties 		colour;
	double 					min_x, max_x, min_y, max_y;
	
	extract_Frame_props(name,position,rotation,node);
	extract_Surface_props(refl_prop,colour,node);
	extract_Plane_props(min_x, max_x, min_y, max_y,node);

	check_name_for_multiple_usage(mother,name);	

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
	check(node, "set_frame");
	check(node, "set_surface");
	check(node, "set_sphere");

	std::string 			name;
	Vector3D 				position;
	Rotation3D 				rotation;
	ReflectionProperties 	refl_prop; 
	ColourProperties 		colour;
	double 					radius;

	extract_Frame_props(name,position,rotation,node);
	extract_Surface_props(refl_prop,colour,node);
	extract_Sphere_props(radius,node);

	check_name_for_multiple_usage(mother,name);

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
	check(node, "set_frame");
	check(node, "set_surface");
	check(node, "set_cylinder");

	std::string 			name;
	Vector3D 				position;
	Rotation3D 				rotation;
	ReflectionProperties 	refl_prop; 
	ColourProperties 		colour;
	double 					cyl_radius;
	Vector3D 				start_of_cylinder, end_of_cylinder;
					
	extract_Frame_props(name,position, rotation,node);
	extract_Surface_props(refl_prop, colour,node);
	extract_Cylinder_props(cyl_radius, start_of_cylinder, end_of_cylinder,node);

	check_name_for_multiple_usage(mother,name);

	Cylinder *new_Cylinder;
	new_Cylinder = new Cylinder;
	new_Cylinder->set_frame(name,position, rotation);
	new_Cylinder->set_surface_properties(&refl_prop, &colour);
	new_Cylinder->set_cylinder(cyl_radius, start_of_cylinder, end_of_cylinder);
	
	mother->set_mother_and_child(new_Cylinder);
	return new_Cylinder;
}
//------------------------------------------------------------------------------
CartesianFrame* WorldFactory::produceFactReflector(
	CartesianFrame* mother,const pugi::xml_node node
){
	check(node, "set_frame");	
	check(node, "set_FACT_reflector");
	
	std::string 		name;
	Vector3D 			position;
	Rotation3D 			rotation;
	double 				alpha;
					
	extract_Frame_props(name,position,rotation,node);
	extract_FACT_props(alpha,node);

	check_name_for_multiple_usage(mother,name);

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
	check(node, "set_frame");	
	check(node, "set_surface");
	check(node, "set_disc");

	std::string 			name;
	Vector3D 				position;
	Rotation3D 				rotation;
	ReflectionProperties 	refl_prop; 
	ColourProperties 		colour;
	double 					radius;

	extract_Frame_props(name, position, rotation, node);
	extract_Surface_props(refl_prop, colour,node);
	extract_Disc_props(radius, node);

	check_name_for_multiple_usage(mother, name);
	
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
	check(node, "set_frame");
	check(node, "set_surface");
	check(node, "set_triangle");
	
	std::string 			name;
	Vector3D 				position;
	Rotation3D 				rotation;
	ReflectionProperties 	refl_prop; 
	ColourProperties 		colour;
	Vector3D 				point_A, point_B, point_C;

	extract_Frame_props(name, position, rotation,node);
	extract_Surface_props(refl_prop, colour,node);
	extract_Triangle_props(point_A, point_B, point_C, node);

	check_name_for_multiple_usage(mother, name);

	Triangle *new_Triangle;
	new_Triangle = new Triangle;			
	new_Triangle->set_frame(name,position,rotation);
	new_Triangle->set_surface_properties(&refl_prop, &colour);
	new_Triangle->set_Triangle(point_A, point_B, point_C);
	
	mother->set_mother_and_child(new_Triangle);
	return new_Triangle;
}
//------------------------------------------------------------------------------
void WorldFactory::extract_Surface_props(
	ReflectionProperties &refl_prop, 
	ColourProperties &colour,
	const pugi::xml_node node
){
	check(node, "set_surface", "refl");
	check(node, "set_surface", "colour");

	std::string refl_attribure = 
		node.child("set_surface").attribute("refl").value();

	// In case the reflection attribute string ends with '.xml', then the
	// reflection coefficient is parsed in out of a xml file. 
	// There the reflection coefficient can be defined as a function of the
	// wavelength. 
	if( is_ending(refl_attribure, ".xml") ){
		// set wavelength depending reflection coefficient
		std::cout << refl_attribure<<std::endl;
		refl_prop.SetReflectionCoefficient( (absolute_path + refl_attribure) );
	}else{
		// set reflection coefficient independent of wavelength	
		refl_prop.SetReflectionCoefficient(pedantic_strtod(refl_attribure));
	}	

	// color properties for camera rays to render images of the scenery
	tuple3 VecTuple; 
	strto3tuple(
		VecTuple,node.child("set_surface").attribute("colour").value()
	);
	colour.set_colour_0to255(VecTuple.x,VecTuple.y,VecTuple.z);

	if(verbose){
		std::stringstream out;
		out << "set_surface (";
		out << "reflection : "<<refl_prop<<", ";
		out << "colour: "<<colour<<" )";
		cout << out.str() << endl;
	}
}
//------------------------------------------------------------------------------
void WorldFactory::check(
	const pugi::xml_node node,
	const std::string child_name,
	const std::string attribute_name
)const{
	if(
		node.child( child_name.c_str() ).attribute( attribute_name.c_str() ) 
		== NULL
	){
		std::stringstream info;
		info << "WorldFactory: Missing attribute. The Node: '" << child_name;
		info << "' requires '" << attribute_name << "'.";
		throw MissingItem( info.str(), this, attribute_name);
	}
}
//------------------------------------------------------------------------------
void WorldFactory::check(
	const pugi::xml_node node,
	const std::string child_name
)const{
	if(node.child( child_name.c_str() ) == NULL){
		std::stringstream info;
		info << "WorldFactory: Missing Node.";
		throw MissingItem( info.str(), this, child_name);
	}
}
//------------------------------------------------------------------------------
void WorldFactory::extract_Frame_props(
	std::string &name, Vector3D &position, Rotation3D &rotation,
	const pugi::xml_node node
){	
	check(node, "set_frame", "name");
	check(node, "set_frame", "pos");	
	check(node, "set_frame", "rot");	

	name = node.child("set_frame").attribute("name").value();

	tuple3 VecTuple; 
	// check position
	strto3tuple(
		VecTuple,node.child("set_frame").attribute("pos").value()
	);

	position.set(VecTuple.x,VecTuple.y,VecTuple.z);

	//check rotation
	strto3tuple(
		VecTuple,node.child("set_frame").attribute("rot").value()
	);

	rotation.set(VecTuple.x,VecTuple.y,VecTuple.z);
	
	if(verbose){
		std::stringstream out;
		out << "set_frame (name: " << name;
		out << ",pos: " << position;
		out << ",rot: " << rotation << ")";
		cout << out.str() << endl;
	}
}
//------------------------------------------------------------------------------
void WorldFactory::extract_Plane_props(
	double &min_x, double &max_x, double &min_y, double &max_y,
	const pugi::xml_node node
){
	check(node, "set_plane", "min_x");	
	check(node, "set_plane", "max_x");	
	check(node, "set_plane", "min_y");	
	check(node, "set_plane", "max_y");	
			
	min_x = pedantic_strtod(
		node.child("set_plane").attribute("min_x").value()
	);

	max_x = pedantic_strtod(
		node.child("set_plane").attribute("max_x").value()
	);

	min_y = pedantic_strtod(
		node.child("set_plane").attribute("min_y").value()
	);

	max_y = pedantic_strtod(
		node.child("set_plane").attribute("max_y").value()
	);

	if(verbose){
		std::stringstream out;
		out << "set_plane (";
		out << "min_x: " << min_x << ", ";
		out << "max_x: " << max_x << ", ";
		out << "min_y: " << min_y << ", ";
		out << "max_y: " << max_y << ")";
		cout << out.str() << endl;
	}
}
//------------------------------------------------------------------------------
void WorldFactory::extract_Sphere_props(
	double &radius,const pugi::xml_node node
){
	check(node, "set_sphere", "radius");	
	
	radius = pedantic_strtod(
		node.child("set_sphere").attribute("radius").value()
	);	

	if(verbose){
		std::stringstream out;
		out << "set_sphere (";
		out << "radius "<<radius<<"[m])";
		cout << out.str() << endl;
	}
}
//------------------------------------------------------------------------------
void WorldFactory::extract_Cylinder_props(
	double &cylinder_radius,
	Vector3D &start_of_cylinder,
	Vector3D &end_of_cylinder,
	const pugi::xml_node node
){
	check(node, "set_cylinder", "radius");
	check(node, "set_cylinder", "start_pos");
	check(node, "set_cylinder", "end_pos");
		
	cylinder_radius = pedantic_strtod(
		node.child("set_cylinder").attribute("radius").value()
	);
	
	// check start_of_cylinder
	tuple3 VecTuple;

	strto3tuple(
		VecTuple,node.child("set_cylinder").attribute("start_pos").value()
	);
	start_of_cylinder.set(VecTuple.x,VecTuple.y,VecTuple.z);
	
	// check end_of_cylinder
	strto3tuple(
		VecTuple,node.child("set_cylinder").attribute("end_pos").value()
	);
	end_of_cylinder.set(VecTuple.x,VecTuple.y,VecTuple.z);
	
	if(verbose){
		std::stringstream out;
		out << "set_cylinder (";
		out << "radius "<<cylinder_radius<<"[m], ";
		out << "start_pos: "<<start_of_cylinder<<", ";
		out << "end_pos: "<<end_of_cylinder<<")";
		cout << out.str() << endl;
	}
}
//------------------------------------------------------------------------------
void WorldFactory::extract_FACT_props(
	double &alpha,const pugi::xml_node node
){
	if(node.child("set_FACT_reflector").attribute("alpha") == NULL){
		
		std::stringstream info;
		info << "set_FACT_reflector requires the 'alpha' statement!";
		info << " Alpha=0 is a Davies-Cotton Reflector, alpha=1 is ";
		info << "a paraboloid reflector. Alpha in between 0 and 1 ";
		info << "is a mixture of both.";
		//throw MissingItem( "alpha", info.str() );
	}
	
	alpha = pedantic_strtod(
		node.child("set_FACT_reflector").attribute("alpha").value()
	);

	if(verbose){
		std::stringstream out;
		out << "set_fact_reflector (";
		out << "alpha "<<alpha<<")";
		cout << out.str() << endl;
	}
}
//------------------------------------------------------------------------------
void WorldFactory::extract_Disc_props(double &radius,const pugi::xml_node node){

	check(node, "set_disc", "radius");

	radius = pedantic_strtod(
		node.child("set_disc").attribute("radius").value()
	);
	
	if(verbose){
		std::stringstream out;
		out << "set_Disc (";
		out << "radius "<<radius<<"[m])";
		cout << out.str() << endl;
	}
}
//------------------------------------------------------------------------------
void WorldFactory::extract_Triangle_props(
	Vector3D &point_A,
	Vector3D &point_B,
	Vector3D &point_C,
	const pugi::xml_node node
){
	check(node, "set_triangle", "A");
	check(node, "set_triangle", "B");
	check(node, "set_triangle", "C");
	
	tuple3 VecTuple; 	
	
	// check point A
	strto3tuple(
		VecTuple,node.child("set_triangle").attribute("A").value()
	);
	point_A.set(VecTuple.x,VecTuple.y,VecTuple.z);

	// check point B
	strto3tuple(
		VecTuple,node.child("set_triangle").attribute("B").value()
	);
	point_B.set(VecTuple.x,VecTuple.y,VecTuple.z);

	// check point C 
	strto3tuple(
		VecTuple,node.child("set_triangle").attribute("C").value()
	);
	point_C.set(VecTuple.x,VecTuple.y,VecTuple.z);

	if(verbose){
		std::stringstream out;
		out << "set_Triangle (";
		out << "A: " << point_A << ", B: " << point_B << ",C: " << point_C;
		out << ")";
		cout << out.str() << endl;
	}
}
//------------------------------------------------------------------------------
void WorldFactory::check_name_for_multiple_usage(
	const CartesianFrame *mother,
	const std::string name_of_additional_child
)const{
	if( 
		mother->get_pointer_to_specific_child(name_of_additional_child)
		 != nullptr
	){
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
void WorldFactory::strto3tuple(tuple3 &tuple, const std::string text)const{
	
	std::string TextToWorkOn = text;
	
	std::size_t pos = TextToWorkOn.find("[");
	if(pos != std::string::npos){
		TextToWorkOn = TextToWorkOn.substr(pos+1);	
	}else{
		std::stringstream info;
		info << "Error parsing tuple: [float,float,float]\n";
		info << "I can not find the opening '[' in '" << text << "' !\n";
		throw BadAttribute(info.str(), this, text);
	}
	
	pos = TextToWorkOn.find(",");
	if(pos != std::string::npos){
		tuple.x = pedantic_strtod(TextToWorkOn.substr(0,pos));	
		TextToWorkOn = TextToWorkOn.substr(pos+1);
	}else{
		std::stringstream info;
		info << "Error parsing tuple: [float,float,float]\n"; 
		info << "I can not find the comma ',' in '" << text << "' !\n";
		throw BadAttribute(info.str(), this, text);
	}
	
	pos = TextToWorkOn.find(",");
	if(pos != std::string::npos){
		tuple.y = pedantic_strtod(TextToWorkOn.substr(0,pos));
		TextToWorkOn = TextToWorkOn.substr(pos+1);
	}else{
		std::stringstream info;
		info << "Error parsing tuple: [float,float,float]\n";  
		info << "I can not find the comma ',' in '" << text << "' !\n";
		throw BadAttribute(info.str(), this, text);
	}	

	pos = TextToWorkOn.find("]");
	if(pos != std::string::npos){
		tuple.z = pedantic_strtod(TextToWorkOn.substr(0,pos));
	}else{
		std::stringstream info;
		info << "Error parsing tuple: [float,float,float]\n";  
		info << "I can not find the closing ']' in '" << text << "' !\n";
		throw BadAttribute(info.str(), this, text);
	}	
}
//------------------------------------------------------------------------------
double WorldFactory::pedantic_strtod(std::string text_to_parse)const{

	if(text_to_parse.compare("") == 0){
		std::stringstream info;
		info << "WorldFactory::"<<__func__<<"() ";
		info << "attribute string is empty.";
		throw BadAttribute(info.str(), this, text_to_parse);
	}
	
	char * e;
	double number_parsed_in = std::strtod(text_to_parse.c_str(), &e);

	if (*e != 0){
		std::stringstream info;
		info << "WorldFactory::"<<__func__<<"() ";
		info << "can not convert attribute string to floating point number.";
		throw BadAttribute(info.str(), this, text_to_parse);
	}
	return number_parsed_in;
}
//------------------------------------------------------------------------------
CartesianFrame* WorldFactory::world(){
	
	root_of_World->post_initialize_me_and_all_my_children();
	root_of_World->update_enclosing_sphere_for_all_children();
	return root_of_World;
}
//------------------------------------------------------------------------------