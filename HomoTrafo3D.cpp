#include "HomoTrafo3D.h"
// homogenous Transformation, component adresses:
// [ 0,0	0,1		0,2		0,3 ]
// [ 1,0	1,1		1,2		1,3 ]
// [ 2,0	2,1		2,2		2,3 ]
// [ 3,0  	3,1		3,2		3,3 ]
//
// -Rotatin component: Matrix R[3x3]
// -Translation component: Vector T[1x3]
//
// homoT = 	[ R(0,0) R(0,1) R(0,2) T(1) ]
//			[ R(1,0) R(1,1) R(1,2) T(2) ]
//			[ R(2,0) R(2,1) R(2,2) T(3) ]
//			[ 0      0      0      1    ]
//
//==================================================================
HomoTrafo3D::HomoTrafo3D(){
	set_unity();
}
//==================================================================
void HomoTrafo3D::set_transformation(const Rotation3D R, const Vector3D pos){

	HomoTrafo3D TrafRotation;
	TrafRotation.set_rotation_component(R);

	HomoTrafo3D TrafTranslation;
	TrafTranslation.set_translation_component(pos);

	// composition	
	*this = TrafTranslation*TrafRotation;
}
//==================================================================
void HomoTrafo3D::set_rotation_component(const Rotation3D R){
	if( R.uses_xyz_angels() ){
		set_rotation_component_based_on_xyz_angles(R);
	}else{
		set_rotation_component_based_on_rot_axis(R);
	}
}
//==================================================================
void HomoTrafo3D::set_translation_component(const Vector3D &t){
	set(0, 3, t.x());
	set(1, 3, t.y());
	set(2, 3, t.z());	
}
//==================================================================
void HomoTrafo3D::set_translation_component_to_zero(){
	set(0, 3, 0.0);
	set(1, 3, 0.0);
	set(2, 3, 0.0);	
}
//==================================================================
void HomoTrafo3D::set_rotation_component_based_on_rot_axis(
	const Rotation3D R
){
		// ensure rot_axis is a unit vector
		Vector3D rot_axis = R.get_rot_axis();
		rot_axis = rot_axis/rot_axis.norm2();
		
		const double rx = rot_axis.x();
		const double ry = rot_axis.y();
		const double rz = rot_axis.z();		
		
		const double sinR = sin( R.get_rot_angle_in_rad() );
		const double cosR = cos( R.get_rot_angle_in_rad() );
		// first row
		set(0,0, cosR +  rx*rx*(1.0-cosR) );
		set(0,1, rx*ry*(1.0-cosR)-rz*sinR );
		set(0,2, rx*rz*(1.0-cosR)+ry*sinR );
		// second row
		set(1,0, ry*rx*(1.0-cosR)+rz*sinR );
		set(1,1, cosR +  ry*ry*(1.0-cosR) );
		set(1,2, ry*rz*(1.0-cosR)-rx*sinR );
		// third row
		set(2,0, rz*rx*(1.0-cosR)-ry*sinR );
		set(2,1, rz*ry*(1.0-cosR)+rx*sinR );
		set(2,2, cosR +  rz*rz*(1.0-cosR) );
}
//==================================================================
void HomoTrafo3D::set_rotation_component_based_on_xyz_angles(
	const Rotation3D R
){
	// first row
	set(0,0, (R.cosRy()*R.cosRz()));
	set(0,1, R.cosRx()*R.sinRz() + R.sinRx()*R.sinRy()*R.cosRz());
	set(0,2, R.sinRx()*R.sinRz() - R.cosRx()*R.sinRy()*R.cosRz());
	// second row
	set(1,0,-R.cosRy()*R.sinRz());
	set(1,1, R.cosRx()*R.cosRz() - R.sinRx()*R.sinRy()*R.sinRz());
	set(1,2, R.sinRx()*R.cosRz() + R.cosRx()*R.sinRy()*R.sinRz());
	// third row
	set(2,0, R.sinRy());
	set(2,1,-R.sinRx()*R.cosRy());
	set(2,2, R.cosRx()*R.cosRy());
}
//==================================================================
void HomoTrafo3D::set_transformation(
	Vector3D rot_x,
	Vector3D rot_y,
	Vector3D rot_z,
	const Vector3D pos
){	
	// normalize rotation vectors
	rot_x = rot_x/rot_x.norm2();
	rot_y = rot_y/rot_y.norm2();
	rot_z = rot_z/rot_z.norm2();
		
	HomoTrafo3D TrafRotation;
	TrafRotation.set_x_column_of_rotation_component(rot_x);
	TrafRotation.set_y_column_of_rotation_component(rot_y);
	TrafRotation.set_z_column_of_rotation_component(rot_z);		
		
	HomoTrafo3D TrafTranslation;	
	TrafTranslation.set_translation_component(pos);

	// composition
	*this = TrafTranslation*TrafRotation;
}
//==================================================================
void HomoTrafo3D::set_x_column_of_rotation_component(const Vector3D &R){
	set(0, 0, R.x());
	set(1, 0, R.y());
	set(2, 0, R.z());	
}
void HomoTrafo3D::set_y_column_of_rotation_component(const Vector3D &R){
	set(0, 1, R.x());
	set(1, 1, R.y());
	set(2, 1, R.z());	
}
void HomoTrafo3D::set_z_column_of_rotation_component(const Vector3D &R){
	set(0, 2, R.x());
	set(1, 2, R.y());
	set(2, 2, R.z());	
}
//==================================================================
void HomoTrafo3D::transform_orientation(Vector3D* vector)const{
	vector->set(
		//x
		vector->x()*get(0,0) + 
		vector->y()*get(0,1) +
		vector->z()*get(0,2),
		//y
		vector->x()*get(1,0) + 
		vector->y()*get(1,1) + 
		vector->z()*get(1,2),
		//z
		vector->x()*get(2,0) + 
		vector->y()*get(2,1) + 
		vector->z()*get(2,2)
	);
}
//==================================================================
void HomoTrafo3D::transform_position(Vector3D* vector)const{
	vector->set(
		//x
		vector->x()*get(0,0) + 
		vector->y()*get(0,1) + 
		vector->z()*get(0,2) + 1.0*get(0,3),
		//y
		vector->x()*get(1,0) + 
		vector->y()*get(1,1) + 
		vector->z()*get(1,2) + 1.0*get(1,3),
		//z
		vector->x()*get(2,0) + 
		vector->y()*get(2,1) + 
		vector->z()*get(2,2) + 1.0*get(2,3)
	);		
}
//==================================================================
Vector3D HomoTrafo3D::get_translation() const{
	Vector3D vec_translation;
	vec_translation.set(get(0, 3), get(1, 3), get(2, 3));
	return vec_translation;
}
//==================================================================
void HomoTrafo3D::disp() const{
	std::cout << get_string();
}
//==================================================================
std::string HomoTrafo3D::get_string() const{
	std::stringstream out; 
	out << std::setprecision(3);
	out << get_single_row_print(0);
	out << get_single_row_print(1);
	out << get_single_row_print(2);
	out << get_single_row_print(3);
	return  out.str();
}
//==================================================================
std::string HomoTrafo3D::get_single_row_print(const uint r)const{
	std::stringstream out; 
	out << std::setprecision(3) <<
	"[  "<<get(r,0)<<" \t"<<get(r,1)<<" \t"<<get(r,2)<<" \t"<<get(r,3)<<"  ]\n";
	return out.str();
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
	HomoTrafo3D I_Rot;
	I_Rot.copy_inverse_rotation_component_from(this);

	HomoTrafo3D I_tra;
	I_tra.copy_inverse_translation_component_from(this);

	// composition
	return I_Rot*I_tra;
}
//==================================================================
void HomoTrafo3D::copy_inverse_rotation_component_from(const HomoTrafo3D *T){
	// Transpose rot matrix because rot maticies are orthogonal
	// and therefore rot^(-1) = rot^T
	// first row of Rot matrix
	set(0, 0, T->get(0, 0));
	set(0, 1, T->get(1, 0));
	set(0, 2, T->get(2, 0));
	// second row of Rot matrix
	set(1, 0, T->get(0, 1));
	set(1, 1, T->get(1, 1));
	set(1, 2, T->get(2, 1));
	// third row of Rot matrix
	set(2, 0, T->get(0, 2));
	set(2, 1, T->get(1, 2));
	set(2, 2, T->get(2, 2));
}
//==================================================================
void HomoTrafo3D::copy_inverse_translation_component_from(const HomoTrafo3D *T){
	// flip sign of translation vector to inverse the effect of the 
	// translation component
	set(0, 3, -1.0*T->get(0, 3));
	set(1, 3, -1.0*T->get(1, 3));
	set(2, 3, -1.0*T->get(2, 3));
}
//==================================================================
void HomoTrafo3D::set(const int row, const int col, const double value){
	E[row*4+col] = value;
}
//==================================================================
double HomoTrafo3D::get(const int row, const int col)const{
	return E[row*4+col];
}	
//==================================================================
void HomoTrafo3D::set_unity(){
	for(int row=0; row<4; row++)
		for(int col=0; col<4; col++)
			set( col, row, ( col == row ) ? 1.0 : 0.0 );
}
//==================================================================
bool HomoTrafo3D::operator== (HomoTrafo3D G)const{
	int counter_which_increases_in_case_of_missmatch = 0;
	for(int row=0; row<4; row++){
		for(int col=0; col<4; col++){
			if( get(col,row) != G.get(col,row) )
				counter_which_increases_in_case_of_missmatch++;
		}
	}
	return (counter_which_increases_in_case_of_missmatch == 0)? true : false;
}
//==================================================================
