#include "CartesianFrame.h"
//==============================================================================
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
		
		T_frame2world_t = 
		ptr_to_mother -> T_frame2mother*T_frame2world_t;
		ptr_to_mother = ptr_to_mother->mother; 
	}

	T_frame2world = T_frame2world_t;
	T_world2frame = T_frame2world_t.inverse();	
	
	// set world pos after all transformations took place
	pos_in_world = T_frame2world.get_translation();

	/* 
	When there are more then max_number_of_frames_in_OctTree children frames 
	in a frame, the children frames are stored in a oct tree structure to
	minimize the costly intersection test 
	*/
	if(children.size() >= max_number_of_frames_in_OctTree){
		/*
		std::cout << "In frame " << name_of_frame;
		std::cout << " an OctTree is created because there are ";
		std::cout << children.size() << " children in this frame." << endl;
		*/
		// create an octree
		OctTree = new OctTreeCube;
		Vector3D center(0.0,0.0,0.0);
		
		SetOctTree(
			OctTree,
			center,
			2.0*radius_of_sphere_enclosing_all_children
		);

		FillOctTree(OctTree,children);

		//std::cout << prompt_OctTree_including_children(OctTree,0);
	}else{
		OctTree = NULL;
	}
}
//==============================================================================
void CartesianFrame::SetOctTree(
	OctTreeCube *Cube,   
    Vector3D CubesCenterPosition,
    double LengthOfEdge){

    Cube->CenterPosition = CubesCenterPosition;
    Cube->EdgeLength = LengthOfEdge;

    double HalfTheEdge = LengthOfEdge/2.0;

    Cube->limits[0][0] = Cube->CenterPosition.x() - HalfTheEdge;
    Cube->limits[0][1] = Cube->CenterPosition.x() + HalfTheEdge;

    Cube->limits[1][0] = Cube->CenterPosition.y() - HalfTheEdge;
    Cube->limits[1][1] = Cube->CenterPosition.y() + HalfTheEdge;

    Cube->limits[2][0] = Cube->CenterPosition.z() - HalfTheEdge;
    Cube->limits[2][1] = Cube->CenterPosition.z() + HalfTheEdge;	
}
//==============================================================================
void CartesianFrame::FillOctTree(
	OctTreeCube *Cube,  
	std::vector<CartesianFrame*> ChildrenToFillIn
){

	Cube->ChildFrames = ChildrenToFillIn;

    if(ChildrenToFillIn.size() > max_number_of_frames_in_OctTree){

    	double ChildCubeEdgeLength = 
    	CalculateEdgeLengthOfChildCube(Cube);

        //std::cout<<"Center Position " <<Cube->CenterPosition<<endl;

        for(uint x=0;x<2;x++){
            for(uint y=0;y<2;y++){
                for(uint z=0;z<2;z++){
                    //std::cout << "x" << x << " y"<<y<<" z"<<z<<endl;

                    //cube_itterator++;
                    OctTreeCube * ChildCube;
                    ChildCube = new OctTreeCube;

                    // calculate CubesCenterPosition position of child cube

                    Vector3D ChildCubesCenterPosition = 
                    CalculateCentrePositionOfChildCube(Cube,x,y,z);

                    SetOctTree(
                    	ChildCube,
                    	ChildCubesCenterPosition,
                    	ChildCubeEdgeLength
                    );

                    //std::cout << "Childs center position ";
                    //std::cout << ChildCubesCenterPosition<<endl;
                    //std::cout << "Childs edge length "
                    //std::cout << ChildCubeEdgeLength <<"[m]"<<endl;

                    std::vector<CartesianFrame*> SubSetOfFrames = 
                    CalculateSubSetOfFramesInCube(
                    	ChildCube,
                    	ChildrenToFillIn
                    );

                    FillOctTree(ChildCube,SubSetOfFrames);

                    Cube->ChildCubes.push_back(ChildCube);
                }
            }
        }
    }
}
//==============================================================================
std::vector<CartesianFrame*> CartesianFrame::CalculateSubSetOfFramesInCube(
	OctTreeCube *Cube,
	std::vector<CartesianFrame*> possible_children
){

	std::vector<CartesianFrame*> SubSetOfFrames;

    for(CartesianFrame* child : possible_children){

    	Vector3D position_of_frame_in_mother_frame = 
    	*child->get_pointer_to_position_of_frame_in_mother_frame();

    	double radius_of_frame = 
    	*(child->get_pointer_to_radius_of_sphere_enclosing_all_children());

    	// range of the bounding sphere of a frame in 
    	// its mother frame coordinates

    	double Xmin = 
    	position_of_frame_in_mother_frame.x() - radius_of_frame;

    	double Xmax = 
    	position_of_frame_in_mother_frame.x() + radius_of_frame;

    	double Ymin = 
    	position_of_frame_in_mother_frame.y() - radius_of_frame;

    	double Ymax = 
    	position_of_frame_in_mother_frame.y() + radius_of_frame;

     	double Zmin = 
    	position_of_frame_in_mother_frame.z() - radius_of_frame;

    	double Zmax = 
    	position_of_frame_in_mother_frame.z() + radius_of_frame;

    	if( (
    		Xmax >= Cube->limits[0][0] && 
    		Xmin <= Cube->limits[0][1] ) && (
    		Ymax >= Cube->limits[1][0] && 
    		Ymin <= Cube->limits[1][1] ) && (
    		Zmax >= Cube->limits[2][0] && 
    		Zmin <= Cube->limits[2][1] ) 
    	){
    		SubSetOfFrames.push_back(child);
    	}
    }
    return SubSetOfFrames;
}
//==============================================================================
Vector3D CartesianFrame::CalculateCentrePositionOfChildCube(
	OctTreeCube *Cube, 
	uint x,uint y,uint z
)const{
    
    Vector3D offset(
        -Cube->EdgeLength/4.0 + x*Cube->EdgeLength/2.0,
        -Cube->EdgeLength/4.0 + y*Cube->EdgeLength/2.0,
        -Cube->EdgeLength/4.0 + z*Cube->EdgeLength/2.0
    );

    Vector3D ChildCubesCenterPosition = Cube->CenterPosition + offset;

    return ChildCubesCenterPosition;
}
//-----------------------------------------------------------------------------
double CartesianFrame::CalculateEdgeLengthOfChildCube(OctTreeCube *Cube)const{
    return Cube->EdgeLength/2.0;
}
//==============================================================================
std::string CartesianFrame::prompt_OctTree_including_children(
	OctTreeCube *Cube,
	unsigned depth
)const{

	if(Cube == NULL) 
		return "There is no OctTree in this frame!";

    std::stringstream out;
    out.str("");  

    std::string gap;
    for(
        unsigned depth_iterator = 0;
        depth_iterator<depth;
        depth_iterator++
    ){
        gap +="    ";
        //out<<"gap iterator: "<<depth_iterator<<std::endl;
    }
    
    //out<<"depth: "<<depth<<std::endl;
    out << gap << " _____OctTreeCube_____" << std::endl;
    out << gap << "| pos in mother   : "<< Cube->CenterPosition << std::endl;
    out << gap << "| edge length     : "<<Cube->EdgeLength <<" [m]"<< std::endl;
    out << gap << "| depth           : "<< depth << std::endl;
    out << gap << "| child cubes     : "<< Cube->ChildCubes.size() << std::endl;   
	out << gap << "| Number of Frames: "<< Cube->ChildFrames.size()<<std::endl;
    // children

	if(Cube->ChildCubes.size() != 0){
	    depth++;

	    uint child_it = 0;
        for( uint x=0; x<2; x++ ){
            for( uint y=0; y<2; y++ ){
                for( uint z=0; z<2; z++ ){
                	out << gap<< "x" << x << " y" << y << " z" << z <<std::endl;
                	out << prompt_OctTree_including_children(
						Cube->ChildCubes.at(child_it),
						depth
					);
                	child_it++;  
                }
            }
        }
	}

	out << gap << "|_____________________" << std::endl;

    return out.str();
}
//==============================================================================
void CartesianFrame::set_frame(
	const std::string new_name,
	const Vector3D npos,
	const Rotation3D nrot
){

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
}
//==============================================================================
void CartesianFrame::disp()const{
	std::stringstream out;
	out.str("");
	out<<"frame_:"<<name_of_frame<<"_____________________________"<<std::endl;
	out<<get_frame_string();
	std::cout<<out.str();
}
//==============================================================================
std::string CartesianFrame::get_frame_string()const{
	std::stringstream out;
	out.str("");
	out << "| pos in mother = " << position_relative_to_mother << std::endl;
	out << "| rot in mother = " << rotation_relative_to_mother << std::endl;
	out << "| pos in world  = " << pos_in_world << std::endl;
	out << "| max radius = ";
	out << radius_of_sphere_enclosing_all_children << " [m]" << std::endl;
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
//==============================================================================
std::string CartesianFrame::get_frame_prompt_including_children(
	unsigned depth
)const{
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
		out<<gap<<"| child "<<(1+i)<<": ";
		out<<children.at(i)->name_of_frame<<std::endl;
		
		out<< children.at(i)->
		get_frame_prompt_including_children(depth);
	}
	return out.str();
}
//==============================================================================
std::string CartesianFrame::get_frame_prompt_including_children()const{
	return get_frame_prompt_including_children(0);
}
//==============================================================================
void CartesianFrame::add_mother(CartesianFrame *const new_mother){
	mother = new_mother;
}
//==============================================================================
void CartesianFrame::add_child(CartesianFrame * const new_child){
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
//==============================================================================
void CartesianFrame::set_mother_and_child(CartesianFrame *new_child){
	this->add_child(new_child);
	new_child->add_mother(this);
}
//==============================================================================
void CartesianFrame::post_initialize_me_and_all_my_children(){
	// post initialize all frames in world.
	// This has to be done to ensure the relationship 
	// declarations are set.
	// rekursiv
	// std::cout<<"post initialization of: "<<name_of_frame<<std::endl;
	post_initializing();
	// and all children
	if(children.size() > 0){
		//this frame has children
		for(CartesianFrame* child : children){
			 child->post_initialize_me_and_all_my_children();
		}
	}
}
//==============================================================================
void CartesianFrame::post_initialize_radius_of_sphere_enclosing_all_children(){
	
	// calculate max norm radius with each child seperatley
	// and choose the biggest radius
	double new_max_norm_radius = 
	radius_of_sphere_enclosing_all_children;
	
	for(CartesianFrame *child : children){

		child->post_initialize_radius_of_sphere_enclosing_all_children();

		double max_norm_radius_of_child = 
		child->radius_of_sphere_enclosing_all_children;

		double distance_between_child_and_this_frame = 
		child->position_relative_to_mother.norm2();

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
//==============================================================================
void  CartesianFrame::take_children(CartesianFrame *frame_to_take_chidren_from){

	// take all children of the frame_to_take_chidren_from and 
	// put them to this frame

	for(CartesianFrame *single_child_to_take_from_frame_to_take_chidren_from:
		frame_to_take_chidren_from->children){
		set_mother_and_child(
			single_child_to_take_from_frame_to_take_chidren_from
		);
	}
}
//==============================================================================
const CartesianFrame* CartesianFrame::get_pointer_to_specific_frame( 
	std::string path 
)const{
	/// Input is a string called path. It is string of concatenated sub strings 
	/// seperated by delimiter '/'. The sub strings are the names of the frames.
	/// The delimiter represents the tree structure of mother and children 
	/// frames.
	/// All frame tree structure is meant to start with a root frame called 
	/// world. This frame is not explicitly indicated in the paths. It is only
	/// indicated by a leading delimiter symbol.
	/// eg.      /house/roof/chimney/chimney_wall_2 insead of
	///     world/house/roof/chimney/chimney_wall_2

	// remove leading delimiter
	if( path.find( "/" ) == 0 )
		path.erase(0,1);

	// Cut out first token of the path
	std::string FirstToken;
	CutOutFirstToken( &path, &FirstToken, "/");

	// Get the pointer to the child with the name given in FirstToken
	const CartesianFrame* SpecificChild = 
	get_pointer_to_specific_child( FirstToken );

	// Check whether there is a child with the name of FirstToken or not
	if( SpecificChild != NULL ){
		// There is a child of this frame which has the same name as FirstToken
		if( path.length() == 0 ){
			// There is no path left, so this actually the final child 
			// we are looking for
			return SpecificChild;
		}else{
			// There is still path to check. We have not reached our final
			// Frame yet so lets proceed with the child
			return SpecificChild -> get_pointer_to_specific_frame( path );
		}
	}else{
		// there is no child on this frame which matches the name in FirstToken
		// This is: There is no such Frame in this Tree of frames as described
		// in path.
		return NULL;
	}
}
//==============================================================================
const CartesianFrame* CartesianFrame::get_pointer_to_specific_child( 
	std::string specific_name 
)const{

	for( CartesianFrame* child : children ){
		// Note: Of course it must be only one child in here with the name given
		// in specific_name!
		if( child->name_of_frame.compare(specific_name) == 0 )
			return child;
	}
	return NULL;
}
//==============================================================================
std::string CartesianFrame::get_path()const{
	/// The path of a frame is returned here. The root frame called world is not 
	/// included in the path. The delimiter sign is '/' as for directorys on 
	/// unix systems.
	/// eg. City/Street14/house18/roof/chimney/chimney_wall_2

	// chech if this frame has a mother frame 
	if( mother != NULL ){
		// This frame has a mother. Therefore it is not the root frame. 
		// Here we add at least the delimiter to the path and ,at least there 
		// is one, the path of its mother
		return mother -> get_path() + "/" +  name_of_frame;
	}else{
		// This frame has not a mother. So this is the root frame. Here is 
		// nothing added to the string
		return "";
	}
}
//==============================================================================