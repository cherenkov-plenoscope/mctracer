//=================================
// include guard
#ifndef __CARTESIANFRAME_H_INCLUDED__
#define __CARTESIANFRAME_H_INCLUDED__

//=================================
// forward declared dependencies
class Ray;
class Intersection;
//=================================
// included dependencies
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "Core/Vec3.h"
#include "Core/Rot3.h"
#include "Core/HomTra3.h"
#include "Core/TracerException.h"
#include "Tools/StringTools.h"
#include "Tools/Tools.h"
#include "Core/Printable.h"
using std::string;
using std::stringstream;
using std::vector;

class Frame : public Printable{
    // The Frame is the fundamental geometry in this framework.
    // It defines a three dimensinal space where rays can be propagated in.
    //
    // The basic frame can not intersect with a ray because it has no surface
    // but the surface entity which inherets from Frame does.
    //
    // A frame defines the geometric relation to its mother frame and its 
    // children frames. This way a tree structure of the scenery is created.
    // The root of this tree is often called 'world' or 'world frame' here.
protected:

    // a frame has a human readable name e.g. Tree, House, Mirror
    string name;

    Vec3 	pos_in_mother; 
    Rot3 	rot_in_mother;
    double 	    radius_of_sphere_enclosing_all_children; 
    Vec3 	pos_in_world;
    
    HomTra3 T_frame2mother;
    HomTra3 T_mother2frame;
    HomTra3 T_world2frame;
    HomTra3 T_frame2world;
    
    vector<Frame*> children;
	Frame *mother = void_frame;
    const Frame *root_frame;
private:

    static const char path_delimiter = '/';
public:

    static const uint max_number_of_children;
    static const double minimal_structure_size;
    static Frame* void_frame;

    //SET
    Frame();
    Frame(const string name, const Vec3 pos, const Rot3 rot);
    void set_name_pos_rot(const string name, const Vec3 pos, const Rot3 rot);
    void set_name(const string name);
    //GET
    string get_name()const;
    string get_path_in_tree_of_frames()const;
    Vec3 get_position_in_mother()const;
    Rot3 get_rotation_in_mother()const;
    Vec3 get_position_in_world()const;
    double contour_radius()const;
    const HomTra3* frame2mother()const;
    const HomTra3* mother2frame()const;
    const HomTra3* world2frame()const;
    const HomTra3* frame2world()const;
    bool has_child_with_name(const string name_of_child)const;
    const Frame* get_child_by_name(string specific_name)const;
    const Frame* get_root_of_world()const;
    uint get_number_of_children()const;
    bool has_mother()const;
    bool has_children()const;
    void assert_no_children_duplicate_names()const;
    virtual string get_print()const;
    string get_tree_print()const;

    //DO
    void init_tree_based_on_mother_child_relations();
    void take_children_from(Frame *frame_to_take_chidren_from);
    void set_mother_and_child(Frame *new_child);
    void find_intersection_candidates_for_all_children_and_ray(
        const Ray* ray,
        vector<const Frame*> *candidate_frames
    )const;
    void update_rotation(const Rot3 rot);
    virtual void calculate_intersection_with(
        const Ray* ray, 
        vector<const Intersection*> *intersections
    )const;
protected:

    // post initialization
    void post_init_root_of_world();
    // post initialization based on root
    void post_init_me_and_all_my_children();
    void post_init_transformations();
    HomTra3 calculate_frame2world()const;
    // post initialization based on mother only
    void post_init_me_and_all_my_children_only_based_on_mother();
    void post_init_transformations_only_based_on_mother();
    HomTra3 calculate_frame2world_only_based_on_mother()const;
    // initialize
    void set_mother(Frame *const new_mother);
    void add_child(Frame * const new_child);
    void update_sphere_enclosing_all_children(Frame *new_child);
    void update_enclosing_sphere_for_all_children();
    void assert_name_is_valid(const string name_to_check)const;
    Vec3 get_mean_pos_in_mother(vector<Frame*> frames)const;
    public: void cluster_using_helper_frames();
    bool positions_in_mother_are_too_close_together(vector<Frame*> frames)const;
    void warn_about_neglection_of(const Frame* frame)const;
public:

    class DuplicateChildName :public TracerException{
        using TracerException::TracerException;
    };

    class BadName :public TracerException{
        using TracerException::TracerException;
    };
};
#endif // __CARTESIANFRAME_H_INCLUDED__