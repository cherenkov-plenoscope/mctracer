//=================================
// include guard
#ifndef __CARTESIANFRAME_H_INCLUDED__
#define __CARTESIANFRAME_H_INCLUDED__

//=================================
// forward declared dependencies
class Intersection;
class ReflectionProperties;
//=================================
// included dependencies
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "Vector3D.h"
#include "Rotation3D.h"
#include "HomoTrafo3D.h"
#include "ColourProperties.h"
#include "TracerException.h"
#include "OctTreeCube.h"
#include "Functions.h"

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

    Vector3D 	position_relative_to_mother; 
    Rotation3D 	rotation_relative_to_mother;
    double 	    radius_of_sphere_enclosing_all_children; 
    Vector3D 	pos_in_world;
    
    HomoTrafo3D T_frame2mother;
    HomoTrafo3D T_mother2frame;
    HomoTrafo3D T_world2frame;
    HomoTrafo3D T_frame2world;
    
    std::vector<CartesianFrame*> children;
	CartesianFrame *mother;

    // OctTree sub structure
    // When there are more then 7 children in a frame the childrens are stored
    // in sub structures made out of OctTrees filling the three dimensional
    // space where the children frames live in

    // since there is only an OctTree substructure the pointer to the OctTree
    // is nullptr by default
    OctTreeCube *OctTree = nullptr;

    // the max number of children is constant and definfes the max number of 
    // children which can be assigned to a single OctTreeNode until the 
    // OctTreeNode is divided itself to store the children frames in its own
    // OctTreeNodes.
    const uint max_number_of_frames_in_OctTree = 7;
public:
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    CartesianFrame(){};
    CartesianFrame(
        const std::string new_name,
        const Vector3D    new_pos,
        const Rotation3D  new_rot
    ){ 
        set_frame(new_name, new_pos, new_rot); 
    };
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    void set_frame(
        const std::string new_name,
        const Vector3D npos,
        const Rotation3D nrot
    );
    void set_mother_and_child(CartesianFrame *new_child);
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    void post_initialize_radius_of_sphere_enclosing_all_children();
    void post_initialize_me_and_all_my_children();
    void take_children(CartesianFrame *frame_to_take_chidren_from);
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    const std::string get_name_of_frame()const{ return name_of_frame; };

    const Vector3D* get_pointer_to_position_of_frame_in_mother_frame()const{
        return &position_relative_to_mother;
    };

    const Rotation3D* get_pointer_to_rotation_of_frame_in_mother_frame() const{
        return &rotation_relative_to_mother;
    };

    const double* get_pointer_to_radius_of_sphere_enclosing_all_children()const{
        return &radius_of_sphere_enclosing_all_children;
    };

    const Vector3D* get_pointer_to_position_of_frame_in_world_frame()const{
        return &pos_in_world;
    };

    const HomoTrafo3D* get_pointer_to_T_frame2mother()const{
        return &T_frame2mother;
    };

    const HomoTrafo3D* get_pointer_to_T_mother2frame()const{
        return &T_mother2frame;
    };

    const HomoTrafo3D* get_pointer_to_T_world2frame()const{
        return &T_world2frame;
    };

    const CartesianFrame* get_pointer_to_mother_frame()const{
        return mother;
    };

    const HomoTrafo3D* get_pointer_to_T_frame2world()const{
        return &T_frame2world;
    };

    const CartesianFrame* get_pointer_to_child(
        const int child_position_in_list
    )const{
        return children.at(child_position_in_list);
    };

    const CartesianFrame* get_pointer_to_specific_frame( 
        std::string path
    )const;

    const CartesianFrame* get_pointer_to_specific_child(
        std::string specific_name
    )const;

    std::string get_path()const;

    const int get_number_of_children()const{ return children.size(); };

    std::string get_frame_string()const;

    std::string get_frame_prompt_including_children()const;

    const OctTreeCube* get_OctTree()const{ return OctTree; };

private:
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    std::string get_frame_prompt_including_children(unsigned depth)const;
    void add_mother(CartesianFrame *const new_mother);
    void add_child(CartesianFrame * const new_child);
    void post_initializing();

    // OctTree
    void SetOctTree(
        OctTreeCube *Ptr2OctTree,   
        Vector3D CubesCenterPosition,
        double LengthOfEdge
    );

    void FillOctTree(
        OctTreeCube *Ptr2OctTree,  
        std::vector<CartesianFrame*> ChildrenToFillIn
    );

    Vector3D CalculateCentrePositionOfChildCube(
        OctTreeCube *Ptr2OctTree,
        uint x,
        uint y,
        uint z
    )const;

    double CalculateEdgeLengthOfChildCube(OctTreeCube *Ptr2OctTree)const;

    std::vector<CartesianFrame*> CalculateSubSetOfFramesInCube(
        OctTreeCube *Ptr2OctTree,
        std::vector<CartesianFrame*> possible_children
    );

    std::string prompt_OctTree_including_children(
        OctTreeCube *Ptr2OctTree,
        unsigned depth
    )const;

    std::vector<CartesianFrame*> CalculateHitCandidates(
        Vector3D support,
        Vector3D direction
    )const;
    
public:
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    virtual void hit(
        Vector3D *base,
        Vector3D *dir,
        Intersection *intersection
    )const{};

    virtual bool is_hit()const{ return false; };

    virtual double get_hit_dist()const{ return 0.0; };

    virtual bool get_hit_reflection_flag()const{ return false; };

    virtual ColourProperties get_hit_colour()const{ 
        ColourProperties defcol; return defcol; 
    };

    virtual const ReflectionProperties* get_ptr2_reflection()const{ 
        return nullptr; 
    };

    virtual void get_reflection_direction_in_object_system(Vector3D* vec)const{};

    virtual void get_intersection_vec_in_object_system(Vector3D *inter)const{};

    virtual bool set_spheric_hexag(
        double new_dbl_focal_length,
        double new_dbl_mirror_radius
    ){ return false; };

    virtual void set_surface_properties(
        ReflectionProperties *n_refl, 
        ColourProperties *n_col
    ){};

    virtual void disp()const;
};
#endif // __CARTESIANFRAME_H_INCLUDED__
