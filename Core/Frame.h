// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_FRAME_H_
#define CORE_FRAME_H_

class Ray;
class Intersection;

#include <string>
#include <vector>
#include "Core/Vec3.h"
#include "Core/Rot3.h"
#include "Core/HomTra3.h"
#include "Tools/StringTools.h"
#include "Tools/Tools.h"

class Frame {
    // A frame defines the geometric relation to its mother frame and its
    // children frames. This way a tree structure of the scenery is created.
    // The root of this tree is often called 'world' or 'world frame' here.
 protected:
    std::string name;
    Vec3 pos_in_mother;
    Rot3 rot_in_mother;
    double bounding_sphere_radius;
    HomTra3 T_frame2mother;
    HomTra3 T_frame2world;
    std::vector<Frame*> children;
    Frame *mother;
    const Frame *root_frame;

 public:
    static const char PATH_DELIMITER;
    static const unsigned int MAX_NUMBER_CHILDREN;
    static const double MIN_STRUCTURE_SIZE;
    static Frame VOID_FRAME;

    // SET
    Frame();
    virtual ~Frame();
    void set_name_pos_rot(
        const std::string name,
        const Vec3 pos,
        const Rot3 rot);
    void update_rotation(const Rot3 rot);
    // GET
    std::string get_name()const;
    std::string get_path_in_tree_of_frames()const;
    Vec3 get_position_in_mother()const;
    Rot3 get_rotation_in_mother()const;
    Vec3 get_position_in_world()const;
    double get_bounding_sphere_radius()const;
    const HomTra3* frame2mother()const;
    const HomTra3* frame2world()const;
    const std::vector<Frame*>* get_children()const;
    const Frame* get_root()const;
    bool has_mother()const;
    bool has_children()const;
    void assert_no_children_duplicate_names()const;
    virtual std::string str()const;
    std::string get_tree_print()const;
    // DO
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
        std::vector<Intersection> *intersections)const;

 private:
    HomTra3 calculate_frame2world()const;
    void init_frame2world();
    void init_root();
    void set_name(const std::string name);
    void warn_small_child(const Frame* frame)const;
    void warn_about_close_frames()const;
    void cluster_children();
    void assert_name_is_valid(const std::string name_to_check)const;
    void update_bounding_sphere();
};
#endif  // CORE_FRAME_H_
