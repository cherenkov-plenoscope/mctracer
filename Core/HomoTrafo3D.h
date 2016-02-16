//=================================
// include guard
#ifndef __HOMOTRAFO3D_H_INCLUDED__
#define __HOMOTRAFO3D_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <iomanip> 
#include "Core/Vector3D.h"
#include "Core/Rotation3D.h"

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

class HomoTrafo3D {

	double T[3][4];
public:

	HomoTrafo3D();
	void set_transformation(const Rotation3D R, const Vector3D pos);
	void set_transformation(
		const Vector3D rot_x,
		const Vector3D rot_y,
		const Vector3D rot_z,
		const Vector3D pos
	);
	void set_unity();
	Vector3D get_transformed_orientation(const Vector3D& ori)const;
	void transform_orientation(Vector3D* vector)const;
	Vector3D get_transformed_position(const Vector3D& pos)const;
	void transform_position(Vector3D* position_to_transform)const;
	Vector3D get_translation()const;
	std::string get_print()const;
	void operator= (HomoTrafo3D G);
	HomoTrafo3D operator* (const HomoTrafo3D G)const;
	HomoTrafo3D inverse()const;
	bool operator== (HomoTrafo3D G)const;
private:

	void copy_inverse_translation_component_from(const HomoTrafo3D *M);
	void copy_inverse_rotation_component_from(const HomoTrafo3D *M);
	std::string get_single_row_print(const uint c)const;
	void set_rotation_component_based_on_xyz_angles(const Rotation3D R);
	void set_rotation_component_based_on_rot_axis(const Rotation3D R);
	void set_translation_component(const Vector3D &t);
	void set_rotation_component(const Rotation3D R);
	void set_x_column_of_rotation_component(const Vector3D &R);
	void set_y_column_of_rotation_component(const Vector3D &R);
	void set_z_column_of_rotation_component(const Vector3D &R);
};
#endif // __HOMOTRAFO3D_H_INCLUDED__ 
