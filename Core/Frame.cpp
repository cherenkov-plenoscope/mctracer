#include "Frame.h"
#include "Tools/AssertionTools.h"

const uint Frame::max_number_of_children_in_frame = 16;
const double Frame::minimal_structure_size = 1e-6;
//------------------------------------------------------------------------------
Frame* Frame::void_frame = new Frame(
	"void_frame",
	Vector3D::null,
	Rotation3D::null
);
//------------------------------------------------------------------------------
Frame::Frame(
    const std::string new_name,
    const Vector3D    new_pos,
    const Rotation3D  new_rot
) { 
    set_name_pos_rot(new_name, new_pos, new_rot); 
}
//------------------------------------------------------------------------------
uint Frame::get_number_of_children()const { 
	return children.size(); 
}
//------------------------------------------------------------------------------
void Frame::post_init_transformations() {
	// in the set_name_pos_rot() function the transformation frame2mother has been set
	// using the new relative position to the mother and the new relative
	// orientation/rotation with respect to the mother frame.
	// All other transformations are calculated using this first frame2mother.

	// The mother2frame is just the inverse of the given frame2mother
	T_mother2frame = T_frame2mother.inverse();

	// For the transformations with respect to the root/world frame we have to 
	// recalculate the current frame2mother transformation by concatination of 
	// all the frame2mother transformations of this frame up to the root frame
	T_frame2world = calculate_frame2world();

	// And again world2frame is just the inverse of the frame2world
	T_world2frame = T_frame2world.inverse();	
	
	// Now we set pos_in_world after all transformations took place
	// This is needed for the bounding spheres later on. Since we use spheres 
	// for the pre intersection tests we do not need the relative rotation of 
	// this frame relative to the root frame to be stored in this frame. 
	pos_in_world = T_frame2world.get_translation();
}
//------------------------------------------------------------------------------
void Frame::post_init_transformations_only_based_on_mother() {

	T_mother2frame = T_frame2mother.inverse();
	T_frame2world = calculate_frame2world_only_based_on_mother();
	T_world2frame = T_frame2world.inverse();
	pos_in_world = T_frame2world.get_translation();
}
//------------------------------------------------------------------------------
HomoTrafo3D Frame::calculate_frame2world()const {
	// All parent frames of this frame do already know their frame2mother 
	// relation.
	// To calculate the frame2world relation of this frame we have to 
	// conacatenate all the frame2mother transformations from this frame to the
	// root frame.

	// The first step toward the root is the mother of this frame
	Frame* frame_on_our_way_towards_the_root = mother;
	
	// The starting point for the latter frame2world is this framse frame2motehr
	HomoTrafo3D Trafo_on_our_way_towards_the_root = T_frame2mother;	

	while(frame_on_our_way_towards_the_root != void_frame){
		Trafo_on_our_way_towards_the_root = 
			frame_on_our_way_towards_the_root -> 
			T_frame2mother*Trafo_on_our_way_towards_the_root;

		frame_on_our_way_towards_the_root = 
			frame_on_our_way_towards_the_root->mother; 
	}

	return Trafo_on_our_way_towards_the_root;
}
//------------------------------------------------------------------------------
HomoTrafo3D Frame::calculate_frame2world_only_based_on_mother()const {

	if(has_mother())
		return mother->T_frame2world*T_frame2mother;
	else
		return T_frame2mother;
}
//------------------------------------------------------------------------------
void Frame::set_name_pos_rot(
	const std::string name_of_frame,
	const Vector3D pos_in_mother,
	const Rotation3D rot_in_mother
){
	this->set_name(name_of_frame);
	
	this->pos_in_mother = pos_in_mother;
	this->rot_in_mother = rot_in_mother;

	// The only transformation set yet is the frame2mother. The others are set
	// when the construction of the world tree has finished and the post 
	// initialization is performed.
	T_frame2mother.set_transformation(rot_in_mother, pos_in_mother);

	reset_all_connections_to_children_and_mother();
}
//------------------------------------------------------------------------------
void Frame::set_name(
	const std::string name_of_frame
){
	assert_name_is_valid(name_of_frame);
	this->name_of_frame = name_of_frame;
}
//------------------------------------------------------------------------------
void Frame::reset_all_connections_to_children_and_mother() {
	radius_of_sphere_enclosing_all_children = 0.0;	 
	mother = void_frame;
	root_of_world = this;
	children.clear();
}
//------------------------------------------------------------------------------
void Frame::assert_name_is_valid(const std::string name_to_check)const {
	AssertionTools::text_with_name_is_not_empty_given_context(
		name_to_check, "name_of_frame", 
		"A frame's name must not be empty"
	);
	AssertionTools::text_with_name_has_no_whitespaces_given_context(
		name_to_check, "name_of_frame", 
		"A frame's name must not contain whitespaces"
	);
	AssertionTools::text_with_name_has_no_specific_char_given_context(
		name_to_check, "name_of_frame", delimiter_for_frame_path,
		"The delimiter sign for the frame's tree structure must not be used in "
		"a frame's name."
	);
}
//------------------------------------------------------------------------------
std::string Frame::get_print()const {
	std::stringstream out;
	out << "frame: " << name_of_frame << "\n";
	out << "| pos in mother: " << pos_in_mother << "\n";
	out << "| rot in mother: " << rot_in_mother << "\n";
	out << "| pos in world:  ";
	out << (has_mother() ? pos_in_world.get_print() : "no mother assigned yet");
	out << "\n";
	out << "| enclosing boundary radius: ";
	out << radius_of_sphere_enclosing_all_children << "m\n";	
	return out.str();
}
//------------------------------------------------------------------------------
std::string Frame::get_tree_print()const {
	
	std::stringstream out;
	out << name_of_frame;
	out << ", pos " << pos_in_mother << ", r ";
	out << radius_of_sphere_enclosing_all_children << "m\n";

	for(Frame* child : children)
		out << StringTools::place_first_infront_of_each_new_line_of_second(
			"| ",
			child->get_tree_print()
		);

	return out.str();
}
//------------------------------------------------------------------------------
void Frame::set_mother(Frame *const new_mother) {
	mother = new_mother;
}
//------------------------------------------------------------------------------
void Frame::add_child(Frame * const new_child) {
	children.push_back(new_child);
	update_sphere_enclosing_all_children(new_child);
}
//------------------------------------------------------------------------------
void Frame::update_sphere_enclosing_all_children(
	Frame *new_child
) {
	// When a child frame is added to a frame we have to check if the sphere 
	// enclosing all the frames previous children is also enclosing the new
	// child. In case the old sphere is to small we have to increase its radius
	// to enclose all the childrens again.
	// To do so we calculate the radius needed to enclose only the new child.
	
	// Case: The new child is not enclosed by the previous sphere of this frame
	//-------             _______________                                     //
	//               ____/               \____                                //
	//            __/     \                   \__                             //
	//           /         \  radius             \                            //
    //          |           \  enclosing          |                           //
    //         |             \  all                |                          //
    //         |              \  (previous)        |          new child       //
    //        |  ___           \  children          |            ___          //
	//        |/     \          \                   |          /     \        //
    //        |       |          x------------------|---------|---x   |       //
    //        |\ ___ /         frame                |          \ _._ /.       //
    //        | previous child   .                  |             .   . 
    //         | defining        .                 |              .   .
    //         |  the current    .                 |              .   .
	//          |  radius        .                |               .   .
	//           \__             .             __/                .   .
	//              \___         .        ____/                   .   .
	//                  \________._______/                        .   .
	//                           .                                .   .
	//                           \_________________  ____________/\_ _/
	//                                             \/               V
	//                                   new childs rel. pos    new childs
	//                                      to mother frame       radius
	//                                             \________  _______/
	//                                                      \/
	//                         new radius of sphere enclosing all children   
	// 
	// Here we have to update the radius of the sphere enclosing all children.
	//      
	// Case: The new child is enclosed by the previous sphere of this frame
	//-------
	//                    _______________
	//               ____/               \____                                //
	//            __/     \ radius            \__                             // 
	//           /         \ enclosing           \                            //
    //          |           \  all                |                        
    //         |             \  (previous)         |
    //         |              \  children           |            
    //        |                \           ___      |            
	//        |                 \        /     \    |          
    //        |                  x------|---x   |   |
    //        |                frame     \ ___ /    |          
    //        |                  .      new child   |                 
    //         |                 .                 |                 
    //         |                 .                 |                  
	//          |                .                |                   
	//           \__             .             __/                    
	//              \___         .        ____/                       
	//                  \________________/                            
	//                                                              
	//                             
	// In this case the old radius remains because it is already enclosing the 
	// new child.

	double radius_needed_to_enclose_new_child = 
	new_child->pos_in_mother.norm() + 
	new_child->radius_of_sphere_enclosing_all_children;

	if(	radius_needed_to_enclose_new_child > 
		radius_of_sphere_enclosing_all_children
	)
		radius_of_sphere_enclosing_all_children = 
		radius_needed_to_enclose_new_child;	
}
//------------------------------------------------------------------------------
void Frame::set_mother_and_child(Frame *new_child) {
	this->add_child(new_child);
	new_child->set_mother(this);
}
//------------------------------------------------------------------------------
void Frame::init_tree_based_on_mother_child_relations() {
	
	cluster_using_helper_frames();
	post_init_me_and_all_my_children();
	update_enclosing_sphere_for_all_children();
	post_init_root_of_world();
}
//------------------------------------------------------------------------------
void Frame::post_init_me_and_all_my_children() {
	// post initialize all frames in world tree of frames.
	// This has to be done to ensure the relationships between each frame and
	// the root frame are declared in each frame.
	post_init_transformations();

	// and all children
	for(Frame* child : children)
		child->post_init_me_and_all_my_children();
}
//------------------------------------------------------------------------------
void Frame::post_init_me_and_all_my_children_only_based_on_mother() {
	post_init_transformations_only_based_on_mother();

	// and all children
	for(Frame* child : children)
		child->post_init_me_and_all_my_children_only_based_on_mother();
}
//------------------------------------------------------------------------------
void Frame::update_enclosing_sphere_for_all_children() {
	// when there is already a treee structure of frames and one is adding a
	// new frame later on, makeing it the child of a previous frame, then one
	// has to update the spheres enclosing the children of all mother frames
	// of the frame where the new frame was added to.
	// Here we do this in a recursive way.

	for(Frame *child : children){
		child->update_enclosing_sphere_for_all_children();
		update_sphere_enclosing_all_children(child);
	}
}
//------------------------------------------------------------------------------
void Frame::post_init_root_of_world() {

	if(has_mother())
		root_of_world = mother->get_root_of_world();
	else
		root_of_world = this;

	for(Frame* child : children)
		child->post_init_root_of_world();
}
//------------------------------------------------------------------------------
const Frame* Frame::get_root_of_world()const {
	return root_of_world;
}
//------------------------------------------------------------------------------
void  Frame::take_children_from(
	Frame *frame_to_take_chidren_from
) {
	// take all children of the frame_to_take_chidren_from and 
	// put them to this frame

	for(Frame *child_to_take: frame_to_take_chidren_from->children)
		set_mother_and_child( child_to_take);
}
//------------------------------------------------------------------------------
const Frame* Frame::get_child_by_name( 
	std::string specific_name 
)const{
	for( Frame* child : children )
		if( StringTools::is_equal(child->name_of_frame, specific_name) )
			return child;

	return void_frame;
}
//------------------------------------------------------------------------------
std::string Frame::get_path_in_tree_of_frames()const {
	/// The path of a frame is returned here. The root frame called world is not 
	/// included in the path. The delimiter sign is '/' as for directorys on 
	/// unix systems.
	/// eg. City/Street14/house18/roof/chimney/chimney_wall_2

	// chech if this frame has a mother frame 
	if( has_mother() ){
		// This frame has a mother. Therefore it is not the root frame. 
		// Here we add at least the delimiter to the path and ,at least there 
		// is one, the path of its mother
		return mother->get_path_in_tree_of_frames() + 
			delimiter_for_frame_path + name_of_frame;
	}else{
		// This frame has not a mother. So this is the root frame. Here is 
		// nothing added to the string
		return "";
	}
}
//------------------------------------------------------------------------------
bool Frame::has_child_with_name(const std::string name_of_child)const {
	return get_child_by_name(name_of_child) != void_frame;
}
//------------------------------------------------------------------------------
bool Frame::has_mother()const {
	return mother != void_frame;
}
//------------------------------------------------------------------------------
bool Frame::has_children()const {
	return get_number_of_children() > 0; 
}
//------------------------------------------------------------------------------
#include "Core/Ray.h"
#include "Core/Intersection.h"
//------------------------------------------------------------------------------
const Intersection* Frame::calculate_intersection_with(
	const Ray* ray
)const {
	return empty_intersection();
}
//------------------------------------------------------------------------------
const Intersection* Frame::empty_intersection()const {
	Intersection* intersection;
	intersection = new Intersection();
	return intersection;
}
//------------------------------------------------------------------------------
void Frame::find_intersection_candidates_for_all_children_and_ray(
	const Ray* ray,
	std::vector<const Frame*> *candidate_frames
)const {

	for(Frame *child : children)
		ray->find_intersection_candidates_in_tree_of_frames(
			child, 
			candidate_frames
		);
}
//------------------------------------------------------------------------------
void Frame::cluster_using_helper_frames() {
	
	if(get_number_of_children() > max_number_of_children_in_frame) {
		
		std::vector<Frame*> oct_tree[8];
		std::vector<Frame*> new_children;

		// assign children to octtree
		for(Frame* child : children)
			oct_tree[child->pos_in_mother.get_octant()].push_back(child);

		for(uint sector=0; sector<8; sector++) {

			if(positions_in_mother_are_too_close_together(oct_tree[sector])) {
				// this can not be clustered
				for(Frame* child : oct_tree[sector]) {
					if(child->get_radius_of_sphere_enclosing_all_children() > minimal_structure_size)
						new_children.push_back(child);
					else
						warn_about_neglection_of(child);
				}
			}else{
				// if children assigned to this sector
				if(oct_tree[sector].size() > 0) {

					// create helper sector
					std::stringstream sector_name;
					sector_name << "octant_" << sector;
					
					Vector3D mean_pos_in_mother = get_mean_pos_in_mother(
						oct_tree[sector]
					);

					Frame* helper_frame = new Frame(
						sector_name.str(),
						mean_pos_in_mother,
						Rotation3D::null
					);

					// assign children to helper frame
					for(Frame* sector_child : oct_tree[sector]) {
						if(sector_child->get_radius_of_sphere_enclosing_all_children() > minimal_structure_size) {

							sector_child->pos_in_mother = 
								sector_child->pos_in_mother - mean_pos_in_mother;

							sector_child->T_frame2mother.set_transformation(
								sector_child->rot_in_mother, 
								sector_child->pos_in_mother
							);

							helper_frame->set_mother_and_child(sector_child);
						}else
							warn_about_neglection_of(sector_child);
					}
					// now cluster the helper frame itself
					helper_frame->cluster_using_helper_frames();
					// now add only helper frame to new children
					new_children.push_back(helper_frame);
				}
			}
		}

		children.clear();
		for(Frame* new_helper_frame_child : new_children)
			set_mother_and_child(new_helper_frame_child);
	}
}
//------------------------------------------------------------------------------
void Frame::warn_about_neglection_of(const Frame* frame)const {
	std::stringstream out;
	out << "___Warning___\n";
	out << __FILE__ << " " << __func__ << "(frame) " << __LINE__ << "\n";
	out << "Frame: " << frame->get_name() << " is neglected. ";
	out << "Encolsing radius is below " << minimal_structure_size << "m, i.e. ";
	out << frame->get_radius_of_sphere_enclosing_all_children() << "m.\n";
	std::cout << out.str();	
}
//------------------------------------------------------------------------------
Vector3D Frame::get_mean_pos_in_mother(
	std::vector<Frame*> frames
)const {
	
	Vector3D sum_pos = Vector3D::null;

	for(Frame* frame : frames)
		sum_pos = sum_pos + frame->pos_in_mother;

	return sum_pos/frames.size();
}
//------------------------------------------------------------------------------
bool Frame::positions_in_mother_are_too_close_together(
	std::vector<Frame*> frames
)const {

		if(frames.size() < 2)
			return false;

		Vector3D mean_pos_in_mother = get_mean_pos_in_mother(frames);

		Vector3D u = Vector3D::null;
		for(Frame* frame : frames) {
			Vector3D r =  frame->pos_in_mother - mean_pos_in_mother;
			u = u + Vector3D(
				r.x()*r.x(), 
				r.y()*r.y(), 
				r.z()*r.z()
			);
		}

		u = u/frames.size();

		double spread = sqrt(u.norm());

		return spread < minimal_structure_size;
}
//------------------------------------------------------------------------------
std::string Frame::get_name()const{ 
	return name_of_frame; 
}
//------------------------------------------------------------------------------
const Vector3D* Frame::get_position_in_mother()const {
    return &pos_in_mother;
}
//------------------------------------------------------------------------------
const Rotation3D* Frame::get_rotation_in_mother()const {
    return &rot_in_mother;
}
//------------------------------------------------------------------------------
const Vector3D* Frame::get_position_in_world()const {
    return &pos_in_world;
}
//------------------------------------------------------------------------------
double Frame::get_radius_of_sphere_enclosing_all_children()const {
    return radius_of_sphere_enclosing_all_children;
}
//------------------------------------------------------------------------------
const HomoTrafo3D* Frame::frame2mother()const {
    return &T_frame2mother;
}
//------------------------------------------------------------------------------
const HomoTrafo3D* Frame::mother2frame()const {
    return &T_mother2frame;
}
//------------------------------------------------------------------------------
const HomoTrafo3D* Frame::world2frame()const {
    return &T_world2frame;
}
//------------------------------------------------------------------------------
const HomoTrafo3D* Frame::frame2world()const {
    return &T_frame2world;
}
//------------------------------------------------------------------------------
void Frame::update_rotation(const Rotation3D rot) {
	rot_in_mother = rot;
	T_frame2mother.set_transformation(rot_in_mother, pos_in_mother);
	post_init_me_and_all_my_children_only_based_on_mother();	
}
//------------------------------------------------------------------------------