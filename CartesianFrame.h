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
#include "Vector3D.h"
#include "Rotation3D.h"
#include "HomoTrafo3D.h"
#include "TracerException.h"
#include "Tools/StringTools.h"
#include "Tools/Tools.h"

class CartesianFrame {
    // The CartesianFrame is the fundamental geometry in this framework.
    // It defines a three dimensinal space where rays can be propagated in.
    //
    // The basic frame can not ińtersect with a ray because it has no surface
    // but the surface entity which inherets from CartesianFrame does.
    //
    // A frame defines the geometric relation to its mother frame and its 
    // children frames. This way a tree structure of the scenery is created.
    // The root of this tree is often called 'world' or 'world frame' here.
protected:

    // Evey frame has a human readable name e.g. Tree, House, Mirror
    std::string name_of_frame;

    Vector3D 	pos_in_mother; 
    Rotation3D 	rot_in_mother;
    double 	    radius_of_sphere_enclosing_all_children; 
    Vector3D 	pos_in_world;
    
    HomoTrafo3D T_frame2mother;
    HomoTrafo3D T_mother2frame;
    HomoTrafo3D T_world2frame;
    HomoTrafo3D T_frame2world;
    
    std::vector<CartesianFrame*> children;
	CartesianFrame *mother;
    const CartesianFrame *root_of_world;
private:

    static const char delimiter_for_frame_path = '/';
public:
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    CartesianFrame(){};
    
    CartesianFrame(
        const std::string new_name,
        const Vector3D    new_pos,
        const Rotation3D  new_rot
    );

    void set_frame(
        const std::string name_of_frame,
        const Vector3D pos_in_mother,
        const Rotation3D rot_in_mother
    );

    void set_mother_and_child(CartesianFrame *new_child);

    void setup_tree_based_on_mother_child_relations();

    void take_children_from(CartesianFrame *frame_to_take_chidren_from);

    const std::string get_name_of_frame()const{ return name_of_frame; };

    const Vector3D* get_position_of_frame_in_mother_frame()const{
        return &pos_in_mother;
    };

    const Rotation3D* get_rotation_of_frame_in_mother_frame() const{
        return &rot_in_mother;
    };

    const Vector3D* get_position_of_frame_in_world_frame()const{
        return &pos_in_world;
    };

    double get_radius_of_sphere_enclosing_all_children()const{
        return radius_of_sphere_enclosing_all_children;
    };

    const HomoTrafo3D* frame2mother()const{
        return &T_frame2mother;
    };

    const HomoTrafo3D* mother2frame()const{
        return &T_mother2frame;
    };

    const HomoTrafo3D* world2frame()const{
        return &T_world2frame;
    };

    const HomoTrafo3D* frame2world()const{
        return &T_frame2world;
    };

    const CartesianFrame* get_frame_in_tree_by_path(std::string path)const;

    bool has_child_with_name(const std::string name_of_child)const;
    const CartesianFrame* get_child_by_name(std::string specific_name)const;

    const CartesianFrame* get_root_of_world()const;

    std::string get_path_in_tree_of_frames()const;

    uint get_number_of_children()const;

    virtual std::string get_print()const;

    std::string get_frame_print()const;

    std::string get_tree_print()const;

    bool has_mother()const;

    bool has_children()const;

    void find_intersection_candidates_for_all_children_and_ray(
        const Ray* ray,
        std::vector<const CartesianFrame*> *candidate_frames
    )const;
private:
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    // post initialization
    void post_initialize_root_of_world();

    void update_enclosing_sphere_for_all_children();

    void post_initialize_me_and_all_my_children();

    // initialize
    void reset_all_connections_to_children_and_mother();

    void set_mother(CartesianFrame *const new_mother);

    void add_child(CartesianFrame * const new_child);

    void post_initialize_transformations();

    HomoTrafo3D calculate_frame2world()const;

    void update_sphere_enclosing_all_children(CartesianFrame *new_child);

    void assert_name_is_valid(const std::string name_to_check)const;

    void assert_name_is_not_empty(
        const std::string name_to_check
    )const;

    void assert_name_has_no_whitespaces(
        const std::string name_to_check
    )const;

    void assert_name_has_no_delimiter_symbol(
        const std::string name_to_check
    )const;
public:
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    virtual Intersection* calculate_intersection_with(const Ray* ray)const;

    Intersection* empty_intersection()const;
};
#endif // __CARTESIANFRAME_H_INCLUDED__