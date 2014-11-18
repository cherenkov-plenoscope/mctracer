#include "HomoTrafo3D.h"
//==================================================================
HomoTrafo3D::HomoTrafo3D(){
	//default constructor
	// homogenous T
	//  0,0	0,1	0,2	0,3
	//	1,0	1,1	1,2	1,3
	//	2,0	2,1	2,2	2,3
	//	3,0	3,1	3,2	3,3
	// homoT = 	[ 1 0 0 0 	]
	//			[ 0 1 0 0 	]
	//			[ 0 0 1	0 	]
	//			[ 0 0 0 1 	]
	set(0,0,1.0);
	set(0,1,0.0);
	set(0,2,0.0);
	set(0,3,0.0);
	set(1,0,0.0);
	set(1,1,1.0);
	set(1,2,0.0);
	set(1,3,0.0);
	set(2,0,0.0);
	set(2,1,0.0);
	set(2,2,1.0);
	set(2,3,0.0);
	set(3,0,0.0);
	set(3,1,0.0);
	set(3,2,0.0);
	set(3,3,1.0);
}
//==================================================================
void HomoTrafo3D::set_transformation(const Rotation3D R,const Vector3D pos){
	// homogenous T
	//  0,0	0,1	0,2	0,3
	//	1,0	1,1	1,2	1,3
	//	2,0	2,1	2,2	2,3
	//	3,0	3,1	3,2	3,3
	HomoTrafo3D T_Rot;
	HomoTrafo3D T_tra;
	
	//==============================================================
	// check wether to built homoT rotation part using xyz-angles or 
	// by using rot axis vector and corresponding angle
	//==============================================================
	if(R.get_flag_is_rot_angles_xyz()==true){
		//cout<<"Set homoT by using xyz angles."<<endl;
		// HOMO ROTATION MATRIX T_Rot
		// create Rotatin Matrix R_xyz[3x3] in homoT Matrix [4x4]
		// homoT = 	[ R(0,0) R(0,1) R(0,2) 0 ]
		//			[ R(1,0) R(1,1) R(1,2) 0 ]
		//			[ R(2,0) R(2,1) R(2,2) 0 ]
		//			[ 0      0      0      1 ]
		// first line
		T_Rot.set
		(0,0, (R.cosRy()*R.cosRz()));
		
		T_Rot.set
		(0,1, R.cosRx()*R.sinRz() + R.sinRx()*R.sinRy()*R.cosRz());
		
		T_Rot.set
		(0,2, R.sinRx()*R.sinRz() - R.cosRx()*R.sinRy()*R.cosRz());
		
		// second line
		T_Rot.set(1,0,-R.cosRy()*R.sinRz());
		T_Rot.set(1,1, R.cosRx()*R.cosRz() - R.sinRx()*R.sinRy()*R.sinRz());
		T_Rot.set(1,2, R.sinRx()*R.cosRz() + R.cosRx()*R.sinRy()*R.sinRz());
		// third line
		T_Rot.set(2,0, R.sinRy());
		T_Rot.set(2,1,-R.sinRx()*R.cosRy());
		T_Rot.set(2,2, R.cosRx()*R.cosRy());
		// translation part of T_Rot
		T_Rot.set(0,3,0.0);
		T_Rot.set(1,3,0.0);
		T_Rot.set(2,3,0.0);
		// the rest of T_Rot
		T_Rot.set(3,0,0.0);
		T_Rot.set(3,1,0.0);
		T_Rot.set(3,2,0.0);
		T_Rot.set(3,3,1.0);
	}else{
		//cout<<"Set homoT by using rot axis and corresponding angle."<<endl;
		// HOMO ROTATION MATRIX T_Rot
		// create Rotatin Matrix R_xyz[3x3] in homoT Matrix [4x4]
		// homoT = 	[ R(0,0) R(0,1) R(0,2) 0 ]
		//			[ R(1,0) R(1,1) R(1,2) 0 ]
		//			[ R(2,0) R(2,1) R(2,2) 0 ]
		//			[ 0      0      0      1 ]
		
		// ensure rot_axis is a unit vector
		Vector3D rot_axis = R.get_rot_axis();
		rot_axis = rot_axis/rot_axis.norm2();
		
		double rx = rot_axis.x();
		double ry = rot_axis.y();
		double rz = rot_axis.z();		
		
		double rot_angle_in_rad = R.get_rot_angle_in_rad();
		// 
		// calculate cos() and sin()
		double sinR = sin(rot_angle_in_rad);
		double cosR = cos(rot_angle_in_rad);
		
		// first line
		T_Rot.set(0,0, cosR + pow(rx,2.0)*(1.0-cosR) );
		T_Rot.set(0,1, rx*ry*(1.0-cosR)-rz*sinR );
		T_Rot.set(0,2, rx*rz*(1.0-cosR)+ry*sinR );
		
		// second line
		T_Rot.set(1,0, ry*rx*(1.0-cosR)+rz*sinR  );
		T_Rot.set(1,1, cosR + pow(ry,2.0)*(1.0-cosR) );
		T_Rot.set(1,2, ry*rz*(1.0-cosR)-rx*sinR );
		
		// third line
		T_Rot.set(2,0, rz*rx*(1.0-cosR)-ry*sinR   );
		T_Rot.set(2,1, rz*ry*(1.0-cosR)+rx*sinR );
		T_Rot.set(2,2, cosR + pow(rz,2.0)*(1.0-cosR) );
		
		// translation part of T_Rot
		T_Rot.set(0,3,0.0);
		T_Rot.set(1,3,0.0);
		T_Rot.set(2,3,0.0);
		// the rest of T_Rot
		T_Rot.set(3,0,0.0);
		T_Rot.set(3,1,0.0);
		T_Rot.set(3,2,0.0);
		T_Rot.set(3,3,1.0);
	}
	//==============================================================
	// translation 
	//==============================================================
	// create Translation Matrix in homoT Matrix [4x4]
	// homoT = 	[ 1 0 0 T(1) 	]
	//			[ 0 1 0 T(2) 	]
	//			[ 0 0 1	T(3) 	]
	//			[ 0 0 0 1 		]
	
	// Translation vector
	T_tra.set(0,3,pos.x());
	T_tra.set(1,3,pos.y());
	T_tra.set(2,3,pos.z());
	
	HomoTrafo3D rot_tra_comp;
	//rot_tra_comp = T_Rot*T_tra;
	rot_tra_comp = T_tra*T_Rot;
	//rot_tra_comp.disp();
	
	set(0,0,rot_tra_comp.get(0,0));
	set(0,1,rot_tra_comp.get(0,1));
	set(0,2,rot_tra_comp.get(0,2));
	set(0,3,rot_tra_comp.get(0,3));
	
	set(1,0,rot_tra_comp.get(1,0));
	set(1,1,rot_tra_comp.get(1,1));
	set(1,2,rot_tra_comp.get(1,2));
	set(1,3,rot_tra_comp.get(1,3));
	
	set(2,0,rot_tra_comp.get(2,0));
	set(2,1,rot_tra_comp.get(2,1));
	set(2,2,rot_tra_comp.get(2,2));
	set(2,3,rot_tra_comp.get(2,3));
	
	set(3,0,rot_tra_comp.get(3,0));
	set(3,1,rot_tra_comp.get(3,1));
	set(3,2,rot_tra_comp.get(3,2));
	set(3,3,rot_tra_comp.get(3,3));
}
//==================================================================
void HomoTrafo3D::set_transformation(
Vector3D rot_x,
Vector3D rot_y,
Vector3D rot_z,
const Vector3D pos){
	
	// normalize rotation vectors
	rot_x = rot_x/rot_x.norm2();
	rot_y = rot_y/rot_y.norm2();
	rot_z = rot_z/rot_z.norm2();
		
	// homogenous T
	//  0,0	0,1	0,2	0,3
	//	1,0	1,1	1,2	1,3
	//	2,0	2,1	2,2	2,3
	//	3,0	3,1	3,2	3,3
	HomoTrafo3D T_Rot;
	HomoTrafo3D T_tra;

	//~ std::cout<<"Set homoT by using rotated unit vectors."<<std::endl;	
	//==============================================================
	// rotation
	//==============================================================

	// HOMO ROTATION MATRIX T_Rot
	// create Rotatin Matrix R_xyz[3x3] in homoT Matrix [4x4]
	// homoT = 	[ R(0,0) R(0,1) R(0,2) 0 ]
	//			[ R(1,0) R(1,1) R(1,2) 0 ]
	//			[ R(2,0) R(2,1) R(2,2) 0 ]
	//			[ 0      0      0      1 ]
	// first row
	T_Rot.set(0,0,rot_x.x());
	T_Rot.set(1,0,rot_x.y());
	T_Rot.set(2,0,rot_x.z());
	
	// second row
	T_Rot.set(0,1,rot_y.x());
	T_Rot.set(1,1,rot_y.y());
	T_Rot.set(2,1,rot_y.z());	

	// third row
	T_Rot.set(0,2,rot_z.x());
	T_Rot.set(1,2,rot_z.y());
	T_Rot.set(2,2,rot_z.z());	
		
	// translation part of T_Rot
	T_Rot.set(0,3,0.0);
	T_Rot.set(1,3,0.0);
	T_Rot.set(2,3,0.0);
	// the rest of T_Rot
	T_Rot.set(3,0,0.0);
	T_Rot.set(3,1,0.0);
	T_Rot.set(3,2,0.0);
	T_Rot.set(3,3,1.0);
	
	//==============================================================
	// translation 
	//==============================================================
	// create Translation Matrix in homoT Matrix [4x4]
	// homoT = 	[ 1 0 0 T(1) 	]
	//			[ 0 1 0 T(2) 	]
	//			[ 0 0 1	T(3) 	]
	//			[ 0 0 0 1 		]
	
	// Translation vector
	T_tra.set(0,3,pos.x());
	T_tra.set(1,3,pos.y());
	T_tra.set(2,3,pos.z());

	//==============================================================
	// composition
	//==============================================================
	
	HomoTrafo3D rot_tra_comp;
	//rot_tra_comp = T_Rot*T_tra;
	rot_tra_comp = T_tra*T_Rot;
	//rot_tra_comp.disp();
	
	set(0,0,rot_tra_comp.get(0,0));
	set(0,1,rot_tra_comp.get(0,1));
	set(0,2,rot_tra_comp.get(0,2));
	set(0,3,rot_tra_comp.get(0,3));
	
	set(1,0,rot_tra_comp.get(1,0));
	set(1,1,rot_tra_comp.get(1,1));
	set(1,2,rot_tra_comp.get(1,2));
	set(1,3,rot_tra_comp.get(1,3));
	
	set(2,0,rot_tra_comp.get(2,0));
	set(2,1,rot_tra_comp.get(2,1));
	set(2,2,rot_tra_comp.get(2,2));
	set(2,3,rot_tra_comp.get(2,3));
	
	set(3,0,rot_tra_comp.get(3,0));
	set(3,1,rot_tra_comp.get(3,1));
	set(3,2,rot_tra_comp.get(3,2));
	set(3,3,rot_tra_comp.get(3,3));
}
//==================================================================
void HomoTrafo3D::transform_orientation(Vector3D* orientation_to_transform)const{
	orientation_to_transform->set(
	//x
	orientation_to_transform->x()*get(0,0) + 
	orientation_to_transform->y()*get(0,1) +
	orientation_to_transform->z()*get(0,2)
	,
	//y
	orientation_to_transform->x()*get(1,0) + 
	orientation_to_transform->y()*get(1,1) + 
	orientation_to_transform->z()*get(1,2)
	,
	//z
	orientation_to_transform->x()*get(2,0) + 
	orientation_to_transform->y()*get(2,1) + 
	orientation_to_transform->z()*get(2,2)
	);
}
//==================================================================
void HomoTrafo3D::transform_position(Vector3D* position_to_transform)const{
	position_to_transform->set(
	//x
	position_to_transform->x()*get(0,0) + 
	position_to_transform->y()*get(0,1) + 
	position_to_transform->z()*get(0,2) + 1.0*get(0,3)
	,
	//y
	position_to_transform->x()*get(1,0) + 
	position_to_transform->y()*get(1,1) + 
	position_to_transform->z()*get(1,2) + 1.0*get(1,3)
	,
	//z
	position_to_transform->x()*get(2,0) + 
	position_to_transform->y()*get(2,1) + 
	position_to_transform->z()*get(2,2) + 1.0*get(2,3)
	);		
}
//==================================================================
Vector3D HomoTrafo3D::get_translation() const{
	Vector3D vec_translation;
	vec_translation.set(
	get(0,3),get(1,3),get(2,3));
	return vec_translation;
}
//==================================================================
void HomoTrafo3D::disp() const{
	std::stringstream out; 
	out.str("");
	out<<"homogene Transformation:"<<std::endl;
	out<<get_string();
	std::cout<<out.str();
}
//==================================================================
std::string HomoTrafo3D::get_string() const{
	std::stringstream out; 
	out.str("");
	out<<std::setprecision(3);
	out<<"[  "<<get(0,0);
	out<<" \t"<<get(0,1);
	out<<" \t"<<get(0,2);
	out<<" \t"<<get(0,3)<<"  ]"<<std::endl;
	out<<"[  "<<get(1,0);
	out<<" \t"<<get(1,1);
	out<<" \t"<<get(1,2);
	out<<" \t"<<get(1,3)<<"  ]"<<std::endl;
	out<<"[  "<<get(2,0);
	out<<" \t"<<get(2,1);
	out<<" \t"<<get(2,2);
	out<<" \t"<<get(2,3)<<"  ]"<<std::endl;
	out<<"[  "<<get(3,0);
	out<<" \t"<<get(3,1);
	out<<" \t"<<get(3,2);
	out<<" \t"<<get(3,3)<<"  ]"<<std::endl;
	return  out.str();
}
//==================================================================
void HomoTrafo3D::operator= (const HomoTrafo3D G){
	for(int i=0; i<16; i++){
		E[i]=G.E[i];
	}
}
//==================================================================
HomoTrafo3D HomoTrafo3D::operator* (const HomoTrafo3D G) const{
	// Matrix multiplication 
	HomoTrafo3D  T;
	for(int x=0 ; x<4 ; x++){
		for(int y=0; y<4; y++){
			T.set(x,y,(
				get(x,0)*G.get(0,y) +
				get(x,1)*G.get(1,y) +
				get(x,2)*G.get(2,y) +
				get(x,3)*G.get(3,y) )
			);
		}
	}	
	return T;
}
//==================================================================
HomoTrafo3D HomoTrafo3D::inverse() const{
	HomoTrafo3D I;
	HomoTrafo3D I_Rot;
	HomoTrafo3D I_tra;
	// homogenous T
	//  0,0	0,1	0,2	0,3
	//	1,0	1,1	1,2	1,3
	//	2,0	2,1	2,2	2,3
	//	3,0	3,1	3,2	3,3
	//
	// 	3,0	3,1	3,2	3,3 = [0 0 0 1] always

	//	to inverse T one has to inverse the rot matrix and inverse
	// 	the signs of translation vector t
	//
	// inverse rot matrix:
	// Transpose rot matrix because rot maticies are orthogonal
	// and therefore rot^(-1) = rot^T
	//first line of Rot matrix
	I_Rot.set(0,0,get(0,0));
	I_Rot.set(0,1,get(1,0));
	I_Rot.set(0,2,get(2,0));
	//second line of Rot matrix
	I_Rot.set(1,0,get(0,1));
	I_Rot.set(1,1,get(1,1));
	I_Rot.set(1,2,get(2,1));
	//third line of Rot matrix
	I_Rot.set(2,0,get(0,2));
	I_Rot.set(2,1,get(1,2));
	I_Rot.set(2,2,get(2,2));
	//
	// flip sign of trans vector
	I_tra.set(0,3,-1.0*get(0,3));
	I_tra.set(1,3,-1.0*get(1,3));
	I_tra.set(2,3,-1.0*get(2,3));
	
	// consider composition
	I = I_Rot*I_tra;
	//I.disp();
	return I;
}
//==================================================================
void HomoTrafo3D::set(const int row,const int col,const double value){
	E[row*4+col] = value;
}
//==================================================================
double HomoTrafo3D::get(const int row,const int col) const{
	return E[row*4+col];
}	
//==================================================================
void HomoTrafo3D::set_unity(){
	// unitiy rotation:
	// 1 0 0
	// 0 1 0
	// 0 0 1
	Rotation3D unity_rotation(0.0, 0.0, 0.0);

	// unity translation
	// (0 0 0)^T
	Vector3D unity_translation(0.0, 0.0, 0.0);

	// resulting homo transformation
	// 1 0 0 0
	// 0 1 0 0
	// 0 0 1 0
	// 0 0 0 1
	set_transformation(unity_rotation, unity_translation);
}
//==================================================================
bool HomoTrafo3D::operator== (HomoTrafo3D G)const{

	int count = 0;
	for(int row=0; row<4; row++){
		for(int col=0; col<4; col++){
			if( get(row,col) != G.get(row,col) )
				count++;
		}
	}

	return (count == 0)? true : false;
}