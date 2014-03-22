#include "CartesianFrame.h"
//======================================================================
CartesianFrame::CartesianFrame(const std::string new_name,const Vector3D npos,const Rotation3D nrot){
	set_frame(new_name,npos,nrot);
}
//======================================================================
void CartesianFrame::post_initializing(){
	//initialize eye matrices
	Vector3D   p(0.0,0.0,0.0);
	Rotation3D r(0.0,0.0,0.0);
	T_mother2frame.set_transformation(r,p);
	T_world2frame.set_transformation(r,p);
	
	// set homogenous T_mother2frame
	T_mother2frame = T_frame2mother.inverse();

	// set homogenous T_world2frame
	HomoTrafo3D T_frame2world_t = T_frame2mother;
	CartesianFrame* ptr_to_mother = mother;
	
	while(ptr_to_mother != NULL){
		T_frame2world_t= 
		ptr_to_mother->T_frame2mother*T_frame2world_t;
		ptr_to_mother= ptr_to_mother->mother; 
	}
	T_frame2world = T_frame2world_t;
	T_world2frame = T_frame2world_t.inverse();	
	
	// set world pos after all transformations took place
	pos_in_world = T_frame2world.get_translation();
};
//======================================================================
const std::string* CartesianFrame::
get_pointer_to_name_of_frame() const{
	return &name_of_frame;
}
//======================
const Vector3D* CartesianFrame::
get_pointer_to_position_of_frame_in_mother_frame() const{
	return &position_relative_to_mother;
}	
//======================
const Rotation3D* CartesianFrame::
get_pointer_to_rotation_of_frame_in_mother_frame() const{
	return &rotation_relative_to_mother;
}	
//======================
const double* CartesianFrame::
get_pointer_to_radius_of_sphere_enclosing_all_children() const{
	return &radius_of_sphere_enclosing_all_children;
}
//======================
const Vector3D* CartesianFrame::
get_pointer_to_position_of_frame_in_world_frame() const{
	return &pos_in_world;
}
//======================
const HomoTrafo3D* CartesianFrame::
get_pointer_to_T_frame2mother() const{
	return &T_frame2mother;
}
//======================
const HomoTrafo3D* CartesianFrame::
get_pointer_to_T_mother2frame() const{
	return &T_mother2frame;
}
//======================
const HomoTrafo3D* CartesianFrame::
get_pointer_to_T_world2frame() const{
	return &T_world2frame;
}
//======================
const HomoTrafo3D* CartesianFrame::
get_pointer_to_T_frame2world() const{
	return &T_frame2world;
}
//======================
const CartesianFrame* CartesianFrame::
get_pointer_to_mother_frame() const{
	return mother;
}
//======================
const CartesianFrame* CartesianFrame::
get_pointer_to_child
(const int child_position_in_list)const{
	return children.at(child_position_in_list);
}
//======================
const int CartesianFrame::
get_number_of_children()const{	
	return children.size();
}
//======================
CartesianFrame::
CartesianFrame(){
}
//======================================================================
void CartesianFrame::
set_frame(
const std::string new_name,const Vector3D npos,const Rotation3D nrot){

	// init name_of_frame
	if(new_name.length()==0){
		throw BadValue(
			"CartesianFrame -> set_frame()",
			"name_of_frame",
			"The name of the frame must not be empty!");
	}
	name_of_frame = new_name;
	
	// init position_relative_to_mother and rotation_relative_to_mother
	position_relative_to_mother = npos;
	rotation_relative_to_mother = nrot;
	// max radius
	radius_of_sphere_enclosing_all_children = 0.0;
	
	// set homogenous T_frame2mother
	T_frame2mother.set_transformation(
	rotation_relative_to_mother,
	position_relative_to_mother
	);
	
	// init mother pointer
	mother = NULL;
	// init empty list with pointer to children
	children.clear();
};
//======================================================================
void CartesianFrame::disp()const{
	std::stringstream out;
	out.str("");
	out<<"frame_:"<<name_of_frame<<"_____________________________"<<std::endl;
	out<<get_frame_string();
	std::cout<<out.str();
}
//======================
std::string CartesianFrame::get_frame_string()const{
	std::stringstream out;
	out.str("");
	out<<"| pos in mother = "<<position_relative_to_mother<<std::endl;
	out<<"| rot in mother = "<<rotation_relative_to_mother<<std::endl;
	out<<"| pos in world  = "<<pos_in_world<<std::endl;
	out<<"| max radius = "<<radius_of_sphere_enclosing_all_children<<" [m]"<<std::endl;
	// disp  T_world2frame
	out<<"| T_world2"<<name_of_frame<<std::endl;
	out<<T_world2frame.get_string();
	// disp  T_frame2world
	out<<"| T_"<<name_of_frame<<"2world"<<std::endl;
	out<<T_frame2world.get_string();
	// mother
	if(mother == 0){
		out<<"| mother: 0"<<std::endl;
	}else{
		out<<"| mother: "<<mother->name_of_frame<<std::endl;
	}
	// children
	out<<"| children: "<<children.size()<<std::endl;
	for (unsigned i=0; i<children.size(); i++){
		out<<"| child "<<(1+i)<<": "<<children.at(i)->name_of_frame<<std::endl;
	}
	return out.str();
}
//======================
std::string CartesianFrame::
get_frame_prompt_including_children(unsigned depth)const{
	std::stringstream out;
	out.str("");	
	std::string gap;
	for(unsigned depth_iterator = 0;depth_iterator<depth;depth_iterator++){
		gap +="    ";
		//out<<"gap iterator: "<<depth_iterator<<std::endl;
	}
	
	//out<<"depth: "<<depth<<std::endl;
	out<<gap<<" __name_=_"<<name_of_frame<<"__"<<std::endl;
	out<<gap<<"| pos in mother = "<<position_relative_to_mother<<std::endl;
	out<<gap<<"| rot in mother = "<<rotation_relative_to_mother<<std::endl;
	out<<gap<<"| pos in world  = "<<pos_in_world<<std::endl;
	out<<gap<<"| max radius = ";
	out<<radius_of_sphere_enclosing_all_children<<" [m]"<<std::endl;
	// disp  T_world2frame
	//out<<gap<<"| T_world2"<<name_of_frame<<std::endl;
	//out<<gap<<T_world2frame.get_string();
	// disp  T_frame2world
	//out<<gap<<"| T_"<<name_of_frame<<"2world"<<std::endl;
	//out<<gap<<T_frame2world.get_string();
	// mother
	if(mother == 0){
		out<<gap<<"| mother: 0"<<std::endl;
	}else{
		out<<gap<<"| mother: "<<mother->name_of_frame<<std::endl;
	}
	// children
	depth++;
	out<<gap<<"| children: "<<children.size()<<std::endl;
	for (unsigned i=0; i<children.size(); i++){
		out<<gap<<"| child "<<(1+i)<<": "<<children.at(i)->name_of_frame<<std::endl;
		
		out<< children.at(i)->
		get_frame_prompt_including_children(depth);
	}
	return out.str();
}
//======================
std::string CartesianFrame::
get_frame_prompt_including_children()const{
	return get_frame_prompt_including_children(0);
}
//======================
void CartesianFrame::
add_mother(CartesianFrame *const new_mother){
	mother = new_mother;
}
//======================
void CartesianFrame::
add_child(CartesianFrame * const new_child){
	children.push_back(new_child);
	//===================
	// calculate new childs diameter in this frame
	//===================
		// calculate the distance from the base of this frame
		// to the base of the childs frame.
		double dist_this_base2_child_base = 
		new_child->position_relative_to_mother.norm2();
		// get the diameter of the new child
		double child_diameter = 
		new_child->radius_of_sphere_enclosing_all_children;
		// the max diameter of the new child in this frame is
		double max_diameter_of_new_child_in_this_frame = 
		dist_this_base2_child_base + child_diameter;
	//===================
	// test if the new childs diameter is bigger than the old one
	//===================
	if(	max_diameter_of_new_child_in_this_frame > 
		radius_of_sphere_enclosing_all_children)
		radius_of_sphere_enclosing_all_children = 
		max_diameter_of_new_child_in_this_frame;
}
//======================
void CartesianFrame::
set_mother_and_child(CartesianFrame *new_child){
	this->add_child(new_child);
	new_child->add_mother(this);
}
//======================
void CartesianFrame::
post_initialize_me_and_all_my_children(){
	// post initialize all frames in world.
	// This has to be done to ensure the relationship 
	// declarations are set.
	// rekursiv
	// std::cout<<"post initialization of: "<<name_of_frame<<std::endl;
	post_initializing();
	// and all children
	if(children.size()>0){
		//this frame has children
		for(unsigned int child_itterator=0;
			child_itterator < children.size();
			child_itterator++)
		{
			 children.at(child_itterator)->
			 post_initialize_me_and_all_my_children();
		}
	}
}
//======================
void CartesianFrame::
hit(
Vector3D *base,Vector3D *dir,
Intersection *intersection)const{
	std::cout<<"call of virtual void hit in class frame"<<std::endl;
}
//======================
bool CartesianFrame::
is_hit(){
	return false;}
//======================
double CartesianFrame::
get_hit_dist(){
	return 0;}
//======================
bool CartesianFrame::
get_hit_reflection_flag()const{
	return false;}
//======================
ColourProperties CartesianFrame::
get_hit_colour()const{
	ColourProperties defcol; return defcol;}
//======================
const ReflectionProperties* CartesianFrame::
get_ptr2_reflection()const{
	std::cout << "virtual get_ptr2_reflection() ";
	std::cout << "called in class frame!" << std::endl;
	return NULL;} 
//======================
void CartesianFrame::
get_reflection_direction_in_object_system
(Vector3D* vec){
}
//======================
void CartesianFrame::
get_intersection_vec_in_object_system(Vector3D *inter){
}
//======================
bool CartesianFrame::
set_spheric_hexag(double new_dbl_focal_length,
double new_dbl_mirror_radius){ 
	std::cout<<"virtual set_spheric_hexag() called in class frame!"<<std::endl;
	return false;
}
//======================
void CartesianFrame::
set_surface_properties
(ReflectionProperties *n_refl, ColourProperties *n_col){
	std::cout<<"virtual set_surface_properties() called in class frame!"<<std::endl;
}
//======================
bool CartesianFrame::
get_sensor_flag()const{
	std::cout<<"virtual get_sensor_flag() called in class frame!"<<std::endl;
	return false;
}
//======================
void CartesianFrame::
post_initialize_radius_of_sphere_enclosing_all_children(){
	
	// calculate max norm radius with each child seperatley
	// and choose the biggest radius
	double new_max_norm_radius = 
	radius_of_sphere_enclosing_all_children;
	
	for(
	unsigned child_iterator = 0;
	child_iterator<children.size();
	child_iterator++){
		
		children.at(child_iterator)->
		post_initialize_radius_of_sphere_enclosing_all_children();
		
		double max_norm_radius_of_child = 
		children.at(child_iterator)->
		radius_of_sphere_enclosing_all_children;
		
		double distance_between_child_and_this_frame = 
		children.at(child_iterator)->
		position_relative_to_mother.norm2();
		
		double max_norm_circle_when_only_takeing_this_child_into_acount=
		max_norm_radius_of_child + 
		distance_between_child_and_this_frame;
		
		if(
		max_norm_circle_when_only_takeing_this_child_into_acount
		>
		new_max_norm_radius
		){
			new_max_norm_radius = 
			max_norm_circle_when_only_takeing_this_child_into_acount;
		}
	}
	
	// seting the new radius
	radius_of_sphere_enclosing_all_children = new_max_norm_radius;
}
//======================
