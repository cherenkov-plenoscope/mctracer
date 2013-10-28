#include "Rotation3D.h"
//======================
void Rotation3D::set_rotation(double Phi, double The, double Psi)
{
	flag_euler_angles = true;
	
	while(Phi >= 2*M_PI){Phi = Phi - 2*M_PI;}
	while(The >= 2*M_PI){The = The - 2*M_PI;}
	while(Psi >= 2*M_PI){Psi = Psi - 2*M_PI;}
	
	while(Phi <= -2*M_PI){Phi = Phi + 2*M_PI;}
	while(The <= -2*M_PI){The = The + 2*M_PI;}
	while(Psi <= -2*M_PI){Psi = Psi + 2*M_PI;}
	Rx = Phi;
	Ry = The;
	Rz = Psi;
}
//======================
void Rotation3D::set_rotation
(const Vector3D vec_new_rot_axis,const double dbl_new_rot_angle_on_rad){
	flag_euler_angles = false;
	vec_rot_axis = vec_new_rot_axis;
	vec_rot_axis = vec_rot_axis/vec_rot_axis.norm2();
	dbl_rot_angle_in_rad = dbl_new_rot_angle_on_rad;
}
//======================
Vector3D Rotation3D::get_rot_axis() const{
	if(flag_euler_angles==true)
	{
		std::cout<<"Warning in Rotation-> get_rot_axis() !"<<std::endl;
		std::cout<<"vec_rot_axis was not set!";
		std::cout<<" Returning default ez."<<std::endl;
		Vector3D temp_vec;
		temp_vec.set_unit_vector_z();
		return temp_vec;
	}
	else
	{
		return vec_rot_axis;
	}
}
//======================
double Rotation3D::get_rot_angle_in_rad() const{
	if(flag_euler_angles==true)
	{
		std::cout<<"Warning in Rotation-> get_rot_angle_in_rad() !";
		std::cout<<std::endl;
		std::cout<<"dbl_rot_angle_in_rad was not set!";
		std::cout<<" Returning default 0.0 [rad]."<<std::endl;
		double temp_rot_angle_in_rad =0.0;
		return temp_rot_angle_in_rad;
	}
	else
	{
		return dbl_rot_angle_in_rad;
	}
}
//======================
bool Rotation3D::get_flag_is_rot_angles_xyz() const{
	return flag_euler_angles;
}
//======================
void Rotation3D::disp() const{
	std::stringstream out; 
	if(flag_euler_angles==true)
	{
		out.str("");
		out<<"euler : ("<<Rx<<"|"<<Ry<<"|"<<Rz<<")"<<std::endl;

	}else{
		out.str("");
		out<<"euler : rot-axis: "<<vec_rot_axis.get_string();
		out<<" rot-angle: "<<dbl_rot_angle_in_rad<<std::endl;
	}
	std::cout<<out.str();
}
//======================
std::string Rotation3D::get_string() const{
	std::stringstream out; 
	if(flag_euler_angles==true)
	{
		out.str("");
		out<<"("<<Rx<<"|"<<Ry<<"|"<<Rz<<") [rad]";

	}else{
		out.str("");
		out<<"euler : rot-axis: "<<vec_rot_axis.get_string();
		out<<", rot-angle: "<<dbl_rot_angle_in_rad<<" [rad]";
	}
	return out.str();
}
//======================
double Rotation3D::cosPhi() const {return cos(Rx);}
double Rotation3D::cosThe() const {return cos(Ry);}
double Rotation3D::cosPsi() const {return cos(Rz);}
double Rotation3D::sinPhi() const {return sin(Rx);}
double Rotation3D::sinThe() const {return sin(Ry);}
double Rotation3D::sinPsi() const {return sin(Rz);}
