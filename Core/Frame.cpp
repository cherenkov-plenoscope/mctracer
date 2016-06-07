#include "Frame.h"
#include <set>

const uint Frame::max_number_of_children = 16;
const double Frame::minimal_structure_size = 1e-6;
//------------------------------------------------------------------------------
Frame* Frame::void_frame = new Frame("void_frame", Vec3::null, Rot3::null);
//------------------------------------------------------------------------------
Frame::Frame():radius_of_sphere_enclosing_all_children(0.0), root_frame(this) {}
//------------------------------------------------------------------------------
Frame::Frame(const string name, const Vec3 pos, const Rot3 rot): Frame() { 
    set_name_pos_rot(name, pos, rot); 
}
//------------------------------------------------------------------------------
void Frame::post_init_transformations() {
	T_frame2world = calculate_frame2world();
}
//------------------------------------------------------------------------------
HomTra3 Frame::calculate_frame2world()const {
	if(has_mother())
		return mother->T_frame2world*T_frame2mother;
	else
		return T_frame2mother;
}
//------------------------------------------------------------------------------
void Frame::set_name_pos_rot(const string name,	const Vec3 pos,	const Rot3 rot) {
	this->set_name(name);
	this->pos_in_mother = pos;
	this->rot_in_mother = rot;

	// The only transformation set yet is the frame2mother. The others are set
	// when the construction of the world tree has finished and the post 
	// initialization is performed.
	T_frame2mother.set_transformation(rot_in_mother, pos_in_mother);
}
//------------------------------------------------------------------------------
void Frame::set_name(const string name) {
	assert_name_is_valid(name);
	this->name = name;
	this->name.shrink_to_fit();
}
//------------------------------------------------------------------------------
void Frame::assert_name_is_valid(const string name_to_check)const {
	
	if(name_to_check.empty()) {
		stringstream info;
		info << "Expected name of frame not to be empty, ";
		info << "but actually it is empty.";
		throw BadName(info.str());
	}	

	uint char_pos = 0;
	for(auto single_character : name_to_check) {
		if(isspace(single_character)) {
			stringstream info;
			info << "Expected name of frame '" << name_to_check << "'' ";
			info << "to have no whitespaces, but actual the char at pos ";
			info << char_pos << " is a whitespace: '";
			info << name_to_check << "'.";
			throw BadName(info.str());			
		}
		char_pos++;
	}

	if(StringTools::string_contains_char(name_to_check, path_delimiter)) {
		std::stringstream info;
		info << "Expected name of frame '" << name_to_check << "' ";
		info << "to not contain any char of '" << path_delimiter << "', ";
		info << "but actual it does.";
		throw BadName(info.str());		
	}
}
//------------------------------------------------------------------------------
string Frame::get_print()const {
	stringstream out;
	out << "frame: " << name << "\n";
	out << "| pos in mother: " << pos_in_mother << "\n";
	out << "| rot in mother: " << rot_in_mother << "\n";
	out << "| pos in world:  ";
	out << (has_mother() ? get_position_in_world().get_print() : "no mother assigned yet");
	out << "\n";
	out << "| enclosing boundary radius: ";
	out << radius_of_sphere_enclosing_all_children << "m\n";	
	return out.str();
}
//------------------------------------------------------------------------------
string Frame::get_tree_print()const {
	
	stringstream out;
	out << name;
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
void Frame::update_sphere_enclosing_all_children(Frame *new_child) {
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
	children.shrink_to_fit();
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
		root_frame = mother->get_root_of_world();
	else
		root_frame = this;

	for(Frame* child : children)
		child->post_init_root_of_world();
}
//------------------------------------------------------------------------------
const Frame* Frame::get_root_of_world()const {
	return root_frame;
}
//------------------------------------------------------------------------------
string Frame::get_path_in_tree_of_frames()const {
	/// The path of a frame is returned here. The root frame called world is not 
	/// included in the path. The delimiter sign is '/' as for directorys on 
	/// unix systems.
	/// eg. City/Street14/house18/roof/chimney/chimney_wall_2

	// chech if this frame has a mother frame 
	if( has_mother() ){
		// This frame has a mother. Therefore it is not the root frame. 
		// Here we add at least the delimiter to the path and ,at least there 
		// is one, the path of its mother
		return mother->get_path_in_tree_of_frames() + path_delimiter + name;
	}else{
		// This frame has not a mother. So this is the root frame. Here is 
		// nothing added to the string
		return "";
	}
}
//------------------------------------------------------------------------------
bool Frame::has_mother()const {
	return mother != void_frame;
}
//------------------------------------------------------------------------------
bool Frame::has_children()const {
	return children.size() > 0; 
}
//------------------------------------------------------------------------------
#include "Core/Ray.h"
#include "Core/Intersection.h"
//------------------------------------------------------------------------------
void Frame::calculate_intersection_with(
	const Ray* ray, 
    vector<Intersection> *intersections
)const {}
//------------------------------------------------------------------------------
void Frame::cluster_using_helper_frames() {
	if(children.size() > max_number_of_children) {
		
		vector<Frame*> oct_tree[8];
		vector<Frame*> new_children;

		// assign children to octtree
		for(Frame* child : children)
			oct_tree[child->pos_in_mother.get_octant()].push_back(child);

		for(uint sector=0; sector<8; sector++) {

			if(positions_in_mother_are_too_close_together(oct_tree[sector])) {
				// this can not be clustered
				for(Frame* child : oct_tree[sector]) {
					if(child->contour_radius() > minimal_structure_size)
						new_children.push_back(child);
					else
						warn_about_neglection_of(child);
				}
			}else{
				// if children assigned to this sector
				if(oct_tree[sector].size() > 0) {

					// create helper sector
					Vec3 mean_pos_in_mother = get_mean_pos_in_mother(
						oct_tree[sector]
					);

					Frame* helper_frame = new Frame(
						"octant_"+std::to_string(sector),
						mean_pos_in_mother,
						Rot3::null
					);

					// assign children to helper frame
					for(Frame* sector_child : oct_tree[sector]) {
						if(sector_child->contour_radius() > minimal_structure_size) {

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
	stringstream out;
	out << "___Warning___\n";
	out << __FILE__ << " " << __func__ << "(frame) " << __LINE__ << "\n";
	out << "Frame: " << frame->get_name() << " is neglected. ";
	out << "Contour radius is below " << minimal_structure_size << "m, i.e. ";
	out << frame->contour_radius() << "m.\n";
	std::cout << out.str();	
}
//------------------------------------------------------------------------------
Vec3 Frame::get_mean_pos_in_mother(vector<Frame*> frames)const {
	Vec3 sum_pos = Vec3::null;

	for(Frame* frame : frames)
		sum_pos = sum_pos + frame->pos_in_mother;

	return sum_pos/frames.size();
}
//------------------------------------------------------------------------------
bool Frame::positions_in_mother_are_too_close_together(vector<Frame*> frames)const {

		if(frames.size() < 2)
			return false;

		const Vec3 mean_pos_in_mother = get_mean_pos_in_mother(frames);

		Vec3 u = Vec3::null;
		for(Frame* frame : frames) {
			const Vec3 r =  frame->pos_in_mother - mean_pos_in_mother;
			u = u + Vec3(
				r.x()*r.x(), 
				r.y()*r.y(), 
				r.z()*r.z()
			);
		}

		u = u/frames.size();
		const double spread = sqrt(u.norm());
		return spread < minimal_structure_size;
}
//------------------------------------------------------------------------------
string Frame::get_name()const { 
	return name; 
}
//------------------------------------------------------------------------------
Vec3 Frame::get_position_in_mother()const {
    return pos_in_mother;
}
//------------------------------------------------------------------------------
Rot3 Frame::get_rotation_in_mother()const {
    return rot_in_mother;
}
//------------------------------------------------------------------------------
Vec3 Frame::get_position_in_world()const {
    return T_frame2world.get_translation();
}
//------------------------------------------------------------------------------
double Frame::contour_radius()const {
    return radius_of_sphere_enclosing_all_children;
}
//------------------------------------------------------------------------------
const HomTra3* Frame::frame2mother()const {
    return &T_frame2mother;
}
//------------------------------------------------------------------------------
const HomTra3* Frame::frame2world()const {
    return &T_frame2world;
}
//------------------------------------------------------------------------------
const vector<Frame*>* Frame::get_children()const {
	return &children;
}
//------------------------------------------------------------------------------
void Frame::update_rotation(const Rot3 rot) {
	rot_in_mother = rot;
	T_frame2mother.set_transformation(rot_in_mother, pos_in_mother);
	post_init_me_and_all_my_children();	
}
//------------------------------------------------------------------------------
void Frame::assert_no_children_duplicate_names()const {
	// this also checks for duplicate frames
	std::set<string> unique_set;

	for(Frame* child: children) {
		auto ret = unique_set.insert(child->get_name());
		if(ret.second == false) {
			stringstream info;
			info << __FILE__ << ", " << __LINE__ << "\n";
			info << "The frame '" << get_name();
			info << "' has a duplicate child name '";
			info << child->get_name();
			info << "', maybe even the child is a duplicate itself.";
			throw DuplicateChildName(info.str());
		}
	}

	for(Frame* child: children)
		child->assert_no_children_duplicate_names();
}
//------------------------------------------------------------------------------