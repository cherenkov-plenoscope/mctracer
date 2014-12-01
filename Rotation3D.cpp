#include "Rotation3D.h"
//------------------------------------------------------------------------------
Rotation3D::Rotation3D() {
}
//------------------------------------------------------------------------------
Rotation3D::Rotation3D(double Phi, double The, double Psi) {
	set(Phi,The,Psi);
}
//------------------------------------------------------------------------------
void Rotation3D::set(double Phi, double The, double Psi) {
	flag_rot_angles_xyz = true;
	
	Rx = Phi;
	Ry = The;
	Rz = Psi;
}
//------------------------------------------------------------------------------
void Rotation3D::set_null_rotation() {
 	Rx = 0.0;
 	Ry = 0.0;
  	Rz = 0.0;
}
 //------------------------------------------------------------------------------
Rotation3D::Rotation3D(
	const Vector3D new_rot_axis,
	const double new_rot_angle_in_rad
){
	set(new_rot_axis,new_rot_angle_in_rad);
}
//------------------------------------------------------------------------------
void Rotation3D::set(
	const Vector3D new_rot_axis,
	const double new_rot_angle_in_rad
){
	flag_rot_angles_xyz = false;
	rot_axis = new_rot_axis/new_rot_axis.norm2();
	rot_angle_in_rad = new_rot_angle_in_rad;
}
//------------------------------------------------------------------------------
Vector3D Rotation3D::get_rot_axis()const {
	if(flag_rot_angles_xyz == true) {
		
		std::cout<<"Warning in Rotation-> get_rot_axis() !"<<std::endl;
		std::cout<<"rot_axis was not set!";
		std::cout<<" Returning default ez."<<std::endl;
		Vector3D temp_vec;
		temp_vec.set_unit_vector_z();
		return temp_vec;
	}else{
		return rot_axis;
	}
}
//------------------------------------------------------------------------------
double Rotation3D::get_rot_angle_in_rad()const {
	if(flag_rot_angles_xyz == true) {
		std::cout<<"Warning in Rotation-> get_rot_angle_in_rad() !";
		std::cout<<std::endl;
		std::cout<<"rot_angle_in_rad was not set!";
		std::cout<<" Returning default 0.0 [rad]."<<std::endl;
		double temp_rot_angle_in_rad =0.0;
		return temp_rot_angle_in_rad;
	}else{
		return rot_angle_in_rad;
	}
}
//------------------------------------------------------------------------------
bool Rotation3D::uses_xyz_angels()const {
	return flag_rot_angles_xyz;
}
//------------------------------------------------------------------------------
void Rotation3D::disp()const {
	std::cout << get_string();
}
//------------------------------------------------------------------------------
std::string Rotation3D::get_string()const {
	std::stringstream out; 
	if(flag_rot_angles_xyz == true) {
		
		out << "(" << Rx << " " << Ry << " " << Rz << ") rad";
	}else{
		out << "euler : rot-axis: " << rot_axis << ", rot-angle: ";
		out << rot_angle_in_rad << " rad";
	}
	return out.str();
}
//------------------------------------------------------------------------------
double Rotation3D::get_rot_x()const {return Rx;}
double Rotation3D::get_rot_y()const {return Ry;}
double Rotation3D::get_rot_z()const {return Rz;}
//------------------------------------------------------------------------------
double Rotation3D::cosRx() const {return cos(Rx);}
double Rotation3D::cosRy() const {return cos(Ry);}
double Rotation3D::cosRz() const {return cos(Rz);}
double Rotation3D::sinRx() const {return sin(Rx);}
double Rotation3D::sinRy() const {return sin(Ry);}
double Rotation3D::sinRz() const {return sin(Rz);}
//------------------------------------------------------------------------------
bool Rotation3D::operator == (const Rotation3D& eqRot)const {	
	if( Rx == eqRot.Rx && Ry == eqRot.Ry && Rz == eqRot.Rz) {
		return true;
	}else{
		return false;
	} 
}
//------------------------------------------------------------------------------
// friends of osstream
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const Rotation3D& rot) {
    os << rot.get_string();
	return os;	
}