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

//======================================================================
class CartesianFrame {
protected:
    std::string name_of_frame;
    //uint ID;

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
    OctTreeCube *OctTree;
    uint max_number_of_frames_in_OctTree;
	
//======================================================================
public: ////////////////////////////////////////////////////////////////
//======================================================================
CartesianFrame();
CartesianFrame(const std::string new_name,const Vector3D npos,const Rotation3D nrot);
//======================
void post_initialize_radius_of_sphere_enclosing_all_children();
void post_initialize_me_and_all_my_children();
void take_children(CartesianFrame *frame_to_take_chidren_from);
//======================================================================
//  GET ////////////////////////////////////////////////////////////////
//======================================================================
const std::string* get_pointer_to_name_of_frame() const;
const Vector3D* get_pointer_to_position_of_frame_in_mother_frame() const;
const Rotation3D* get_pointer_to_rotation_of_frame_in_mother_frame() const;
const double* get_pointer_to_radius_of_sphere_enclosing_all_children() const;
const Vector3D* get_pointer_to_position_of_frame_in_world_frame() const;
const HomoTrafo3D* get_pointer_to_T_frame2mother() const;
const HomoTrafo3D* get_pointer_to_T_mother2frame() const;
const HomoTrafo3D* get_pointer_to_T_world2frame() const;
const HomoTrafo3D* get_pointer_to_T_frame2world() const;
const CartesianFrame* get_pointer_to_mother_frame() const;
const CartesianFrame* get_pointer_to_child(const int child_position_in_list)const;
const CartesianFrame* get_pointer_to_specific_frame( std::string path )const;
const CartesianFrame* get_pointer_to_specific_child( std::string specific_name )const;
std::string get_path()const;
const int get_number_of_children()const;
//const uint get_ID() const{ return ID; };
std::string get_frame_string()const;
std::string get_frame_prompt_including_children()const;
//======================================================================
//  SET ////////////////////////////////////////////////////////////////
//======================================================================
void set_frame(const std::string new_name,const Vector3D npos,const Rotation3D nrot);
//void set_ID( uint newID ){ ID = newID; };
void set_mother_and_child(CartesianFrame *new_child);
const OctTreeCube* get_OctTree()const;
//======================================================================
private: ///////////////////////////////////////////////////////////////
//======================================================================
void BasicSetUp();
std::string get_frame_prompt_including_children(unsigned depth)const;
void add_mother(CartesianFrame *const new_mother);
void add_child(CartesianFrame * const new_child);
void post_initializing();
//=====================================================================
// OctTree ////////////////////////////////////////////////////////////
//=====================================================================
void SetOctTree(
    OctTreeCube *Ptr2OctTree,   
    Vector3D CubesCenterPosition,
    double LengthOfEdge);
//======================
void FillOctTree(
    OctTreeCube *Ptr2OctTree,  
    std::vector<CartesianFrame*> ChildrenToFillIn
);
//======================
Vector3D CalculateCentrePositionOfChildCube(OctTreeCube *Ptr2OctTree,uint x,uint y,uint z)const;
//======================
double CalculateEdgeLengthOfChildCube(OctTreeCube *Ptr2OctTree)const;
//======================
std::vector<CartesianFrame*> CalculateSubSetOfFramesInCube(
    OctTreeCube *Ptr2OctTree,
    std::vector<CartesianFrame*> possible_children
);
//======================
std::string prompt_OctTree_including_children(
    OctTreeCube *Ptr2OctTree,
    unsigned depth
)const;
//======================
std::vector<CartesianFrame*> CalculateHitCandidates(
    Vector3D support,
    Vector3D direction
)const;
//======================================================================
public: // VIRTUAL /////////////////////////////////////////////////////
//======================================================================
virtual void hit(Vector3D *base,Vector3D *dir,Intersection *intersection)const;
virtual bool is_hit();
virtual double get_hit_dist();
virtual bool get_hit_reflection_flag()const;
virtual ColourProperties get_hit_colour()const;
virtual const ReflectionProperties* get_ptr2_reflection()const;
virtual void get_reflection_direction_in_object_system(Vector3D* vec);
virtual void get_intersection_vec_in_object_system(Vector3D *inter);
virtual bool set_spheric_hexag(double new_dbl_focal_length,double new_dbl_mirror_radius);
virtual void set_surface_properties(ReflectionProperties *n_refl, ColourProperties *n_col);
virtual bool get_sensor_flag()const;
virtual void disp()const;
};
#endif // __CARTESIANFRAME_H_INCLUDED__
