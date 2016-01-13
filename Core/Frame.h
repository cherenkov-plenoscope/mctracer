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
#include "Core/Vector3D.h"
#include "Core/Rotation3D.h"
#include "Core/HomoTrafo3D.h"
#include "TracerException.h"
#include "Tools/StringTools.h"
#include "Tools/Tools.h"
#include "Core/Printable.h"

class Frame : public Printable{
    // The Frame is the fundamental geometry in this framework.
    // It defines a three dimensinal space where rays can be propagated in.
    //
    // The basic frame can not ińtersect with a ray because it has no surface
    // but the surface entity which inherets from Frame does.
    //
    // A frame defines the geometric relation to its mother frame and its 
    // children frames. This way a tree structure of the scenery is created.
    // The root of this tree is often called 'world' or 'world frame' here.
protected:

    // a frame has a human readable name e.g. Tree, House, Mirror
    std::string name_of_frame;

    Vector3D 	pos_in_mother; 
    Rotation3D 	rot_in_mother;
    double 	    radius_of_sphere_enclosing_all_children; 
    Vector3D 	pos_in_world;
    
    HomoTrafo3D T_frame2mother;
    HomoTrafo3D T_mother2frame;
    HomoTrafo3D T_world2frame;
    HomoTrafo3D T_frame2world;
    
    std::vector<Frame*> children;
	Frame *mother = void_frame;
    const Frame *root_of_world;
private:

    static const char delimiter_for_frame_path = '/';
public:
    static const uint max_number_of_children_in_frame;
    static const double minimal_structure_size;
//------------------------------------------------------------------------------
    static Frame* void_frame;

    Frame();
    Frame(
        const std::string new_name,
        const Vector3D    new_pos,
        const Rotation3D  new_rot
    );
    void set_name_pos_rot(
        const std::string name_of_frame,
        const Vector3D pos_in_mother,
        const Rotation3D rot_in_mother
    );
    void set_name(
        const std::string name_of_frame
    );
    void set_mother_and_child(Frame *new_child);
    void init_tree_based_on_mother_child_relations();
    void take_children_from(Frame *frame_to_take_chidren_from);
    std::string get_name()const;
    const Vector3D* get_position_in_mother()const;
    const Rotation3D* get_rotation_in_mother()const;
    const Vector3D* get_position_in_world()const;
    double get_radius_of_sphere_enclosing_all_children()const;
    const HomoTrafo3D* frame2mother()const;
    const HomoTrafo3D* mother2frame()const;
    const HomoTrafo3D* world2frame()const;
    const HomoTrafo3D* frame2world()const;
    bool has_child_with_name(const std::string name_of_child)const;
    const Frame* get_child_by_name(std::string specific_name)const;
    const Frame* get_root_of_world()const;
    std::string get_path_in_tree_of_frames()const;
    uint get_number_of_children()const;
    virtual std::string get_print()const;
    std::string get_tree_print()const;
    bool has_mother()const;
    bool has_children()const;
    void find_intersection_candidates_for_all_children_and_ray(
        const Ray* ray,
        std::vector<const Frame*> *candidate_frames
    )const;
    void assert_no_children_duplicate_names()const;
protected:
    // post initialization
    void post_init_root_of_world();
    // post initialization based on root
    void post_init_me_and_all_my_children();
    void post_init_transformations();
    HomoTrafo3D calculate_frame2world()const;
    // post initialization based on mother only
    void post_init_me_and_all_my_children_only_based_on_mother();
    void post_init_transformations_only_based_on_mother();
    HomoTrafo3D calculate_frame2world_only_based_on_mother()const;
    // initialize
    void reset_all_connections_to_children_and_mother();
    void set_mother(Frame *const new_mother);
    void add_child(Frame * const new_child);
    void update_sphere_enclosing_all_children(Frame *new_child);
    void update_enclosing_sphere_for_all_children();
    void assert_name_is_valid(const std::string name_to_check)const;
    Vector3D get_mean_pos_in_mother(std::vector<Frame*> frames)const;
    public: void cluster_using_helper_frames();
    bool positions_in_mother_are_too_close_together(std::vector<Frame*> frames)const;
    void warn_about_neglection_of(const Frame* frame)const;
public:
    // moving/rotating the frame after construction
    void update_rotation(const Rotation3D rot);
    virtual const Intersection* calculate_intersection_with(const Ray* ray)const;
    const Intersection* empty_intersection()const;

    class DuplicateChildName :public TracerException{
        using TracerException::TracerException;
    };
};
#endif // __CARTESIANFRAME_H_INCLUDED__