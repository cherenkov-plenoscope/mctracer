//=================================
// include guard
#ifndef __HomTra3_H_INCLUDED__
#define __HomTra3_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <string>
#include "Core/Vec3.h"
#include "Core/Rot3.h"

//=================================
// The concept of Homogeneous Transformations is taken from the text book 
//
// "Robotics -- Modelling, Planning and Control" 
// by Bruno Siciliano, Lorenzo Sciavicco, Luigi Villani and Guiseppe Oriolo.
//
// The chapter on kinematics serves an excellent introduction into 3D concepts
// and is highly recomended for all questions on:
//
// - Pose of a rigid body
// - Rotation Matrix
//   - Elementary Rotations
//   - Representation of a Vector
//   - Rotation of a Vector
// - Composition of Rotation Matrices
// - Euler Angles
// - ZYZ Angles
// - RPY Angles
// - Angle and Axis
// - Unit Quaternion
// - Homogeneous Transformation

class HomTra3 {

	double T[3][4];
public:

	HomTra3();
	void set_transformation(const Rot3 R, const Vec3 pos);
	void set_transformation(Vec3 rotx, Vec3 roty, Vec3 rotz, const Vec3 pos);
	Vec3 get_transformed_orientation(const Vec3& ori)const;
	void transform_orientation(Vec3* vector)const;
	Vec3 get_transformed_position(const Vec3& pos)const;
	void transform_position(Vec3* position_to_transform)const;
	Vec3 get_translation()const;
	std::string get_print()const;
	void operator= (HomTra3 G);
	HomTra3 operator* (const HomTra3 G)const;
	HomTra3 inverse()const;
	bool operator== (HomTra3 G)const;
private:

	void copy_inverse_translation_component_from(const HomTra3 *M);
	void copy_inverse_rotation_component_from(const HomTra3 *M);
	std::string get_single_row_print(const uint c)const;
	void set_rotation_component_based_on_xyz_angles(const Rot3 R);
	void set_rotation_component_based_on_rot_axis(const Rot3 R);
	void set_translation_component(const Vec3 &t);
	void set_rotation_component(const Rot3 R);
	void set_x_column_of_rotation_component(const Vec3 &R);
	void set_y_column_of_rotation_component(const Vec3 &R);
	void set_z_column_of_rotation_component(const Vec3 &R);
};
#endif // __HomTra3_H_INCLUDED__ 
