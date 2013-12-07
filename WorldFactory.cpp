#include "WorldFactory.h"
//=================================
WorldFactory::WorldFactory(){
	root_of_World = new CartesianFrame;
	root_of_World->
	set_frame("world",Vector3D(0.0,0.0,0.0),Rotation3D(0.0,0.0,0.0));
}
//=================================
bool WorldFactory::load_file(std::string filename){
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename.c_str());
	
	std::stringstream out;
	
    if (result){
        out << "XML [" << filename << "] ";
        out << "parsing successful";
		cout << out.str() << endl;
		
		try
		{
			frame_factory(root_of_World,doc);
			return true;
		}
		catch(WorldFactoryException& error)
		{
			error.ReportException();
			return false;
		}
		
    }else{
        out << "XML [" << filename << "] ";
        out << "parsed with errors, attr value: [";
        out << doc.child("node").attribute("attr").value() << "]\n";
        out << "Error description: " << result.description() << "\n";
		out << "Number of character in file: " << result.offset;
		//out << "(error at [..." << (filename + result.offset) << "]\n\n";
		
		cout << out.str() << endl;
		
		return false;
	}
}
//=================================
void WorldFactory::frame_factory(
CartesianFrame* mother,const pugi::xml_node node){
	
	// prompt info
	std::stringstream out;
	out << "frame factory checking node: "<<node.name();
	out << " name=";
	out << node.child("set_frame").attribute("name").value();
	cout << out.str() << endl;
	
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
		
	}else{
		if(mother->get_number_of_children()==0){
			// Ignore the root of the world. 
			// Here no frame pos and rot must be set!
		}else{
			throw UnknowenObject(node_name);
		}
	}
	
	go_on_with_children_of_node(mother,node);
}
//=================================
void WorldFactory::go_on_with_children_of_node
(CartesianFrame* mother,const pugi::xml_node node){
	
	// go on with children of node
	for(pugi::xml_node sub_node = node.first_child(); 
	sub_node; 
	sub_node = sub_node.next_sibling()){	
		
		std::string sub_node_name = sub_node.name();
		bool valid_child = false;
		
		if(sub_node_name.compare("frame")==0){
			frame_factory(mother,sub_node); valid_child=true;}

		if(sub_node_name.compare("triangle")==0){
			frame_factory(mother,sub_node); valid_child=true;}	
			
		if(sub_node_name.compare("plane")==0){
			frame_factory(mother,sub_node); valid_child=true;}
			
		if(sub_node_name.compare("sphere")==0){
			frame_factory(mother,sub_node); valid_child=true;}		

		if(sub_node_name.compare("cylinder")==0){
			frame_factory(mother,sub_node); valid_child=true;}	

		if(sub_node_name.compare("disc")==0){
			frame_factory(mother,sub_node); valid_child=true;}	

		if(sub_node_name.compare("FACT_reflector")==0){
			frame_factory(mother,sub_node); valid_child=true;}	
	
		if(valid_child){
			std::stringstream out;
			out<<"frame_factory -> calling frame_factory for child >";
			out<<sub_node.name()<<"< of node >";
			out<<node.child("set_frame").attribute("name").value()<<"<";
			cout << out.str() << endl;	
		}else{
			if(sub_node_name.find("set") == std::string::npos){
				// this is no "setter" and it is no known object
				throw UnknowenObject(sub_node_name);
			}
		}
	}	
}
//=================================
CartesianFrame* WorldFactory::produceCartesianFrame
(CartesianFrame* mother,const pugi::xml_node node){
	
	pugi::xml_node check_node = node.child("set_frame");
	if(check_node == NULL){
		throw MissingItem("set_frame",
		"A frame requires the 'set_frame' statement!");
	}
	
	std::string name;
	Vector3D position;
	Rotation3D rotation;
	
	set_frame(name,position,rotation,node);
		
	CartesianFrame *frame;
	frame = new CartesianFrame;
	
	frame->set_frame(name,position,rotation);
	mother->set_mother_and_child(frame);
	
	return frame;
}
//=================================
CartesianFrame* WorldFactory::producePlane
(CartesianFrame* mother,const pugi::xml_node node){

	pugi::xml_node check_node = node.child("set_frame");
	if(check_node == NULL){
		throw MissingItem("set_frame",
		"A plane requires the 'set_frame' statement!");
	}
	
	check_node = node.child("set_surface");
	if(check_node == NULL){
		throw MissingItem("set_surface",
		"A plane requires the 'set_surface' statement!");
	}
	
	check_node = node.child("set_plane");
	if(check_node == NULL){
		throw MissingItem("set_plane",
		"A plane requires the 'set_plane' statement!");
	}	
	
	std::string name;
	Vector3D position;
	Rotation3D rotation;
	
	ReflectionProperties reflection_cefficient; 
	ColourProperties colour;
	
	double min_x;
	double max_x;
	double min_y;
	double max_y;
	
	// set plane
	
	Plane *new_plane;
	new_plane = new Plane;
	
	if(!set_frame(name,position,rotation,node)){
		std::stringstream out;
		out<<"producePlane -> set_frame is not valid!";
		cout << out.str() << endl;
	}
		
	if(!set_surface(reflection_cefficient,colour,node)){
		std::stringstream out;
		out<<"producePlane -> set_surface is not valid!";
		cout << out.str() << endl;
	}
	
	if(!set_plane(min_x, max_x, min_y, max_y,node)){
		std::stringstream out;
		out<<"producePlane -> set_plane is not valid!";
		cout << out.str() << endl;		
	}
			
	new_plane->set_frame(name,position,rotation);
	new_plane->set_surface_properties(&reflection_cefficient,&colour);
	new_plane->set_plane(min_x, max_x, min_y, max_y);
	
	mother->set_mother_and_child(new_plane);
	return new_plane;
}
//=================================
CartesianFrame* WorldFactory::produceSphere
(CartesianFrame* mother,const pugi::xml_node node){
	
	std::string name;
	Vector3D 	position;
	Rotation3D 	rotation;
	
	ReflectionProperties reflection_cefficient; 
	ColourProperties colour;
	
	double radius;
	// set sphere
	
	Sphere *new_sphere;
	new_sphere = new Sphere;
	
	if(!set_frame(name,position,rotation,node)){
		std::stringstream out;
		out<<"produceSphere -> set_frame is not valid!";
		cout << out.str() << endl;
	}
		
	if(!set_surface(reflection_cefficient,colour,node)){
		std::stringstream out;
		out<<"produceSphere -> set_surface is not valid!";
		cout << out.str() << endl;
	}
	
	if(!set_sphere(radius,node)){
		std::stringstream out;
		out<<"produceSphere -> set_Sphere is not valid!";
		cout << out.str() << endl;		
	}
			
	new_sphere->set_frame(name,position,rotation);
	new_sphere->set_surface_properties(&reflection_cefficient,&colour);
	new_sphere->set_sphere(radius);
	
	mother->set_mother_and_child(new_sphere);
	return new_sphere;
}
//=================================
CartesianFrame* WorldFactory::produceCylinder
(CartesianFrame* mother,const pugi::xml_node node){
	
	// frame related
	std::string name;
	Vector3D 	position;
	Rotation3D 	rotation;

	// surface related	
	ReflectionProperties reflection_cefficient; 
	ColourProperties colour;
	
	// cylinder related	
	double cylinder_radius;
	Vector3D start_of_cylinder;
	Vector3D end_of_cylinder;
					
	// collect all information for cylinder

	
	if(!set_frame(name,position,rotation,node)){
		std::stringstream out;
		out<<"produceSphere -> set_frame is not valid!";
		cout << out.str() << endl;
	}
		
	if(!set_surface(reflection_cefficient,colour,node)){
		std::stringstream out;
		out<<"produceSphere -> set_surface is not valid!";
		cout << out.str() << endl;
	}
	
	if(!set_Cylinder(
	cylinder_radius,start_of_cylinder,end_of_cylinder,node)){
		std::stringstream out;
		out<<"produceSphere -> set_Sphere is not valid!";
		cout << out.str() << endl;		
	}
	
	Cylinder *new_Cylinder;
	new_Cylinder = new Cylinder;	
			
	new_Cylinder->set_frame(name,position,rotation);
	new_Cylinder->set_surface_properties(
						&reflection_cefficient,&colour);
	new_Cylinder->set_cylinder(
						cylinder_radius,
						start_of_cylinder, 
						end_of_cylinder);
	
	mother->set_mother_and_child(new_Cylinder);
	return new_Cylinder;
}
//=================================
CartesianFrame* WorldFactory::produceFactReflector
(CartesianFrame* mother,const pugi::xml_node node){
	
	// frame related
	std::string name;
	Vector3D 	position;
	Rotation3D 	rotation;

	// fact reflector related	
	double alpha;
					
	// collect all information for cylinder
	if(!set_frame(name,position,rotation,node)){
		std::stringstream out;
		out<<"produceFactReflector -> set_frame is not valid!";
		cout << out.str() << endl;
	}
		
	if(!set_fact_reflector(alpha,node)){
		std::stringstream out;
		out<<"produceFactReflector -> set_fact_reflector is not valid!";
		cout << out.str() << endl;
	}
	
	FactTelescope *new_FactTelescope;
	new_FactTelescope = new FactTelescope(alpha);			
	new_FactTelescope->set_frame(name,position,rotation);
	new_FactTelescope->init();
	
	mother->set_mother_and_child(new_FactTelescope);
	return new_FactTelescope;
}
//=================================
CartesianFrame* WorldFactory::produceDisc
(CartesianFrame* mother,const pugi::xml_node node){

	std::string name;
	Vector3D position;
	Rotation3D rotation;
	
	ReflectionProperties reflection_cefficient; 
	ColourProperties colour;
	
	double radius;

	// set disc
	
	if(!set_frame(name,position,rotation,node)){
		std::stringstream out;
		out<<"produceDisc -> set_frame is not valid!";
		cout << out.str() << endl;
	}
		
	if(!set_surface(reflection_cefficient,colour,node)){
		std::stringstream out;
		out<<"produceDisc -> set_surface is not valid!";
		cout << out.str() << endl;
	}
	
	if(!set_Disc(radius,node)){
		std::stringstream out;
		out<<"produceDisc -> set_disc is not valid!";
		cout << out.str() << endl;		
	}
	
	Disc *new_Disc;
	new_Disc = new Disc;			
	new_Disc->set_frame(name,position,rotation);
	new_Disc->set_surface_properties(&reflection_cefficient,&colour);
	new_Disc->set_Disc(radius);
	
	mother->set_mother_and_child(new_Disc);
	return new_Disc;
}
//=================================
CartesianFrame* WorldFactory::produceTriangle
(CartesianFrame* mother,const pugi::xml_node node){

	std::string name;
	Vector3D position;
	Rotation3D rotation;
	
	ReflectionProperties reflection_cefficient; 
	ColourProperties colour;
	
	Vector3D point_A;
	Vector3D point_B;
	Vector3D point_C;
	// set triangle
	
	if(!set_frame(name,position,rotation,node)){
		std::stringstream out;
		out<<"produceTriangle -> set_frame is not valid!";
		cout << out.str() << endl;
	}
		
	if(!set_surface(reflection_cefficient,colour,node)){
		std::stringstream out;
		out<<"produceTriangle -> set_surface is not valid!";
		cout << out.str() << endl;
	}
	
	if(!set_Triangle(point_A,point_B,point_C,node)){
		std::stringstream out;
		out<<"produceTriangle -> set_disc is not valid!";
		cout << out.str() << endl;		
	}
	
	Triangle *new_Triangle;
	new_Triangle = new Triangle;			
	new_Triangle->set_frame(name,position,rotation);
	new_Triangle->set_surface_properties(&reflection_cefficient,&colour);
	new_Triangle->set_Triangle(point_A,point_B,point_C);
	
	mother->set_mother_and_child(new_Triangle);
	return new_Triangle;

}
//=================================
bool WorldFactory::set_surface(
ReflectionProperties &reflection_cefficient, ColourProperties &colour,
const pugi::xml_node node){
	
	// check reflection coeficient
	double FloatingNumber;
	parseFloatingNumber(
	FloatingNumber,node.child("set_surface").attribute("refl").value());
	reflection_cefficient.set_reflection_coefficient(FloatingNumber);
	
	// check colour
	tuple3 VecTuple; 
	parse3tuple(
	VecTuple,node.child("set_surface").attribute("colour").value());
	colour.set_colour_0to255(VecTuple.x,VecTuple.y,VecTuple.z);

	std::stringstream out;
	out << "set_surface (";
	out << "reflection : "<<reflection_cefficient<<", ";
	out << "colour: "<<colour<<" )";
	cout << out.str() << endl;
	
	return true;
}
//=================================
bool WorldFactory::set_frame(
std::string &name, Vector3D &position, Rotation3D &rotation,
const pugi::xml_node node){
	
	pugi::xml_node check_node = node.child("set_frame");
	if(check_node == NULL){
		throw MissingItem("set_frame",
		"A frame requires the 'set_frame' statement!");
	}
	
	name = node.child("set_frame").attribute("name").value();

	if(!check_name_for_multiple_usage(root_of_World,name)){
		std::stringstream out;
		out << "set_frame can not set name >";
		out << node.child("set_frame").attribute("name").value();
		out<<"< because it is already in use !";
		cout << out.str() << endl;		
		return false;
	}
	
	tuple3 VecTuple; 
	// check position
	parse3tuple(
	VecTuple,node.child("set_frame").attribute("pos").value());
	position.set_vec3D(VecTuple.x,VecTuple.y,VecTuple.z);

	
	//check rotation
	parse3tuple(
	VecTuple,node.child("set_frame").attribute("rot").value());
	rotation.set_rotation(VecTuple.x,VecTuple.y,VecTuple.z);
	
	std::stringstream out;
	out << "set_frame (name: "<<name;
	out <<",pos: "<<position;
	out <<",rot: "<<rotation<<")";
	cout << out.str() << endl;
	return true;
}
//=================================
bool WorldFactory::set_plane(
double &min_x, double &max_x, double &min_y, double &max_y,
const pugi::xml_node node){
	
	pugi::xml_node check_node = node.child("set_plane");
	if(check_node == NULL){
		throw MissingItem("set_plane",
		"A plane requires the 'set_plane' statement!");
	}
	
	double FloatingNumber;
	parseFloatingNumber(
	FloatingNumber,node.child("set_plane").attribute("min_x").value());
	min_x = FloatingNumber;

	parseFloatingNumber(
	FloatingNumber,node.child("set_plane").attribute("max_x").value());
	max_x = FloatingNumber;
	
	parseFloatingNumber(
	FloatingNumber,node.child("set_plane").attribute("min_y").value());
	min_y = FloatingNumber;
	
	parseFloatingNumber(
	FloatingNumber,node.child("set_plane").attribute("max_y").value());
	max_y = FloatingNumber;	
	
//~ min_x = node.child("set_plane").attribute("min_x").as_double();
//~ max_x = node.child("set_plane").attribute("max_x").as_double();
//~ min_y = node.child("set_plane").attribute("min_y").as_double();
//~ max_y = node.child("set_plane").attribute("max_y").as_double();

	std::stringstream out;
	out << "set_plane (";
	out << "min_x: "<<min_x<<", ";
	out << "max_x: "<<max_x<<", ";
	out << "min_y: "<<min_y<<", ";
	out << "max_y: "<<max_y<<")";
	cout << out.str() << endl;

return true;
}
//=================================
bool WorldFactory::set_sphere(
double &radius,const pugi::xml_node node){
	
	pugi::xml_node check_node = node.child("set_sphere");
	if(check_node == NULL){
		throw MissingItem("set_sphere",
		"A sphere requires the 'set_sphere' statement!");
	}
	
	double FloatingNumber;
	parseFloatingNumber(
	FloatingNumber,
	node.child("set_sphere").attribute("radius").value());
	radius = FloatingNumber;
	
	//radius = node.child("set_sphere").attribute("radius").as_double();
	
	std::stringstream out;
	out << "set_sphere (";
	out << "radius "<<radius<<"[m])";
	cout << out.str() << endl;
	return true;
}
//=================================
bool WorldFactory::set_Cylinder(
double &cylinder_radius,
Vector3D &start_of_cylinder,
Vector3D &end_of_cylinder,
const pugi::xml_node node){
	
	pugi::xml_node check_node = node.child("set_cylinder");
	if(check_node == NULL){
		throw MissingItem("set_cylinder",
		"A cylinder requires the 'set_cylinder' statement!");
	}
	
	double FloatingNumber;
	parseFloatingNumber(
	FloatingNumber,
	node.child("set_cylinder").attribute("radius").value());
	cylinder_radius = FloatingNumber;
	
	//~ cylinder_radius = 
	//~ node.child("set_cylinder").attribute("radius").as_double();

	// check start_of_cylinder
	tuple3 VecTuple; 
	parse3tuple(
	VecTuple,node.child("set_cylinder").attribute("start_pos").value());
	start_of_cylinder.set_vec3D(VecTuple.x,VecTuple.y,VecTuple.z);
	
	// check end_of_cylinder
	parse3tuple(
	VecTuple,node.child("set_cylinder").attribute("end_pos").value());
	end_of_cylinder.set_vec3D(VecTuple.x,VecTuple.y,VecTuple.z);
	
	std::stringstream out;
	out << "set_cylinder (";
	out << "radius "<<cylinder_radius<<"[m], ";
	out << "start_pos: "<<start_of_cylinder<<", ";
	out << "end_pos: "<<end_of_cylinder<<")";
	cout << out.str() << endl;
	return true;
}
//=================================
bool WorldFactory::set_fact_reflector(
double &alpha,const pugi::xml_node node){
	
	pugi::xml_node check_node = node.child("set_FACT_reflector");
	if(check_node == NULL){
		throw MissingItem("set_FACT_reflector",
		"The FACT reflector requires the 'set_FACT_reflector' statement!");
	}
	
	double FloatingNumber;
	parseFloatingNumber(
	FloatingNumber,
	node.child("set_FACT_reflector").attribute("alpha").value());
	alpha = FloatingNumber;
	
	//~ alpha = 
	//~ node.child("set_FACT_reflector").attribute("alpha").as_double();
	
	std::stringstream out;
	out << "set_fact_reflector (";
	out << "alpha "<<alpha<<")";
	cout << out.str() << endl;
	return true;
}
//=================================
bool WorldFactory::set_Disc(double &radius,const pugi::xml_node node){

	pugi::xml_node check_node = node.child("set_disc");
	if(check_node == NULL){
		throw MissingItem("set_disc",
		"A disc requires the 'set_disc' statement!");
	}

	double FloatingNumber;
	parseFloatingNumber(
	FloatingNumber,node.child("set_disc").attribute("radius").value());
	radius = FloatingNumber;	

	//~ radius = node.child("set_disc").attribute("radius").as_double();
	
	std::stringstream out;
	out << "set_Disc (";
	out << "radius "<<radius<<"[m])";
	cout << out.str() << endl;
	return true;	
}
//=================================
bool WorldFactory::set_Triangle(
Vector3D &point_A,
Vector3D &point_B,
Vector3D &point_C,
const pugi::xml_node node){
	
	pugi::xml_node check_node = node.child("set_triangle");
	if(check_node == NULL){
		throw MissingItem("set_triangle",
		"A triangle requires the 'set_triangle' statement!");
	}

	tuple3 VecTuple; 	
	
	// check point A
	parse3tuple(
	VecTuple,node.child("set_triangle").attribute("A").value());
	point_A.set_vec3D(VecTuple.x,VecTuple.y,VecTuple.z);

	// check point B
	parse3tuple(
	VecTuple,node.child("set_triangle").attribute("B").value());
	point_B.set_vec3D(VecTuple.x,VecTuple.y,VecTuple.z);

	// check point C 
	parse3tuple(
	VecTuple,node.child("set_triangle").attribute("C").value());
	point_C.set_vec3D(VecTuple.x,VecTuple.y,VecTuple.z);

	std::stringstream out;
	out << "set_Triangle (";
	out << "A: "<<point_A<<", B: "<<point_B<<",C: "<<point_C<<")";
	cout << out.str() << endl;
	return true;
}
//=================================
bool WorldFactory::check_name_for_multiple_usage(
const CartesianFrame *entitie_in_world,std::string name){

	//~ std::stringstream out;
	//~ out<<"check_name_for_multiple_usage() ";
	//~ out<<"check >"<<name<<"< and frame ";
	//~ out<<">";
	//~ out<<*entitie_in_world->get_pointer_to_name_of_frame()<<"<";
	//~ cout << out.str() << endl;
	
	if(
	entitie_in_world->
	get_pointer_to_name_of_frame()->
	compare(name) == 0){
		// this name is already in use!
		std::stringstream out;
		out<<"check_name_for_multiple_usage() ";
		out<<"the name to check >"<<name<<"< is already in use ";
		out<<"by frame >";
		out<<*entitie_in_world->get_pointer_to_name_of_frame()<<"<";
		cout << out.str() << endl;

		return false;
	}else{
		for(int cild_iterator = 0;
		cild_iterator<entitie_in_world->get_number_of_children();
		cild_iterator++)
		{
			return check_name_for_multiple_usage(
			entitie_in_world->get_pointer_to_child(cild_iterator),
			name
			);
		}
		return true;
	}
	
}
//=================================
bool WorldFactory::parse3tuple(tuple3 &tuple,std::string text){
	std::string input_text = text;

	std::string sx;
	std::string sy;
	std::string sz;
	
	std::size_t pos = text.find("[");
	if(pos != std::string::npos){
		text = text.substr(pos+1);
		//std::cout<<"vector construct found >[< the rest is >"<<text<<"<"<<std::endl;	
	}else{
		throw SyntaxError(
		"[float,float,float]",
		input_text,
		"I can not find > [ <");
	}
	
	pos = text.find(",");
	if(pos != std::string::npos){
		sx = text.substr(0,pos);
		//std::cout<<"vector construct found sx >"<<sx<<"<"<<std::endl;	
		text = text.substr(pos+1);
	}else{
		throw SyntaxError(
		"[float,float,float]",
		input_text,
		"I can not find the first comma > , <");
	}
	
	pos = text.find(",");
	if(pos != std::string::npos){
		sy = text.substr(0,pos);
		//std::cout<<"vector construct found sy >"<<sx<<"<"<<std::endl;	
		text = text.substr(pos+1);
	}else{
		throw SyntaxError(
		"[float,float,float]",
		input_text,
		"I can not find the second comma > , <");
	}	

	pos = text.find("]");
	if(pos != std::string::npos){
		sz = text.substr(0,pos);
		//std::cout<<"vector construct found sz >"<<sx<<"<"<<std::endl;	
	}else{
		throw SyntaxError(
		"[float,float,float]",
		input_text,
		"I can not find > ] <");
	}	

	char * e;
	tuple.x = std::strtod(sx.c_str(), &e);
	if (*e != 0) {
		
		std::stringstream out;
		out<<"The string >" << sx << "< ";
		out<<"in argument x ";
		out<<"is not a valid floating point number!";		
		throw SyntaxError("[float,float,float]",input_text,out.str());
	}
	tuple.y = std::strtod(sy.c_str(), &e);
	if (*e != 0) {
		std::stringstream out;
		out<<"The string >" << sy << "< ";
		out<<"in argument y ";
		out<<"is not a valid floating point number!";
		throw SyntaxError("[float,float,float]",input_text,out.str());
	}
	tuple.z = std::strtod(sz.c_str(), &e);
	if (*e != 0) {
		std::stringstream out;
		out<<"The string >" << sz << "< ";
		out<<"in argument z ";
		out<<"is not a valid floating point number!";
		throw SyntaxError("[float,float,float]",input_text,out.str());
	}
	
	return true;
}
//=================================
void WorldFactory::parseFloatingNumber(
double &FloatingNumber,std::string text_to_parse){
	
	if(text_to_parse.compare("")==0){
		throw SyntaxError
		("float",text_to_parse,"The floating number string is empty!");
	}
	
	char * e;
	FloatingNumber = std::strtod(text_to_parse.c_str(), &e);
	if (*e != 0) {
		
		std::stringstream out;
		out<<"The string >" << text_to_parse << "< ";
		out<<"is not a valid floating point number!";		
		throw SyntaxError("float",text_to_parse,out.str());
	}
}
//=================================
CartesianFrame*  WorldFactory::get_pointer_to_world(){
	
	root_of_World->
	post_initialize_me_and_all_my_children();
	
	root_of_World->
	post_initialize_radius_of_sphere_enclosing_all_children();
	
	return root_of_World;
	
}
//=================================
//
