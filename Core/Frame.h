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
using std::string;
using std::stringstream;
using std::vector;

class Frame {
    // A frame defines the geometric relation to its mother frame and its 
    // children frames. This way a tree structure of the scenery is created.
    // The root of this tree is often called 'world' or 'world frame' here.
protected:

    string name;

    Vec3 pos_in_mother; 
    Rot3 rot_in_mother;
    double radius_of_sphere_enclosing_all_children; 
    
    HomTra3 T_frame2mother;
    HomTra3 T_frame2world;
    
    vector<Frame*> children;
	Frame *mother = &void_frame;
    const Frame *root_frame;
public:

    static const char path_delimiter = '/';
    static const uint max_number_of_children;
    static const double minimal_structure_size;
    static Frame void_frame;

    //SET
    Frame();
    virtual ~Frame();
    void set_name_pos_rot(const string name, const Vec3 pos, const Rot3 rot);
    void update_rotation(const Rot3 rot);
    //GET
    string get_name()const;
    string get_path_in_tree_of_frames()const;
    Vec3 get_position_in_mother()const;
    Rot3 get_rotation_in_mother()const;
    Vec3 get_position_in_world()const;
    double contour_radius()const;
    const HomTra3* frame2mother()const;
    const HomTra3* frame2world()const;
    const vector<Frame*>* get_children()const;
    const Frame* get_root()const;
    bool has_mother()const;
    bool has_children()const;
    void assert_no_children_duplicate_names()const;
    virtual string get_print()const;
    string get_tree_print()const;
    //DO
    template<class ProtoFrame>
    ProtoFrame* append() {
        ProtoFrame* child = new ProtoFrame;
        children.push_back(child);
        child->mother = this;
        return child;
    }
    void erase(const Frame* child);
    void init_tree_based_on_mother_child_relations();
    virtual void calculate_intersection_with(
        const Ray* ray, 
        vector<Intersection> *intersections
    )const;    
private:

    HomTra3 calculate_frame2world()const;
    void init_frame2world();
    void init_root();
    void set_name(const string name);
    void warn_small_child(const Frame* frame)const;
    void warn_about_close_frames()const;
    void cluster_children();
    void assert_name_is_valid(const string name_to_check)const;
    void update_boundary_sphere_radius_for_child(Frame *new_child);
    void update_boundary_sphere();
    
public:

    class NoSuchChild: public TracerException{
        using TracerException::TracerException;
    };
    class DuplicateChildName :public TracerException{
        using TracerException::TracerException;
    };
    class BadName :public TracerException{
        using TracerException::TracerException;
    };
    friend std::ostream& operator<<(std::ostream& os, const Frame& frame) {
        os << frame.get_print();
        return os;
    };
};
#endif // __CARTESIANFRAME_H_INCLUDED__