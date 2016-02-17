#include "Core/HomoTrafo3D.h"
// homogenous Transformation, component adresses:
// [ 0,0    0,1     0,2     0,3 ]
// [ 1,0    1,1     1,2     1,3 ]
// [ 2,0    2,1     2,2     2,3 ]
// [ 3,0    3,1     3,2     3,3 ]
//
// -Rotatin component: Matrix R[3x3]
// -Translation component: Vector T[1x3]
//
// homoT =  [ R(0,0) R(0,1) R(0,2) T(1) ]
//          [ R(1,0) R(1,1) R(1,2) T(2) ]
//          [ R(2,0) R(2,1) R(2,2) T(3) ]
//          [ 0      0      0      1    ]
//
//------------------------------------------------------------------------------
HomoTrafo3D::HomoTrafo3D():
    T{{1,0,0,0},{0,1,0,0},{0,0,1,0}}
{
    //default init is unit matrix
    // [1 0 0 0]
    // [0 1 0 0]
    // [0 0 1 0]
    // [0 0 0 1] //last row is always the same
}
//------------------------------------------------------------------------------
void HomoTrafo3D::set_transformation(const Rotation3D R, const Vector3D pos) {

    HomoTrafo3D TrafRotation;
    TrafRotation.set_rotation_component(R);

    HomoTrafo3D TrafTranslation;
    TrafTranslation.set_translation_component(pos);

    // composition  
    *this = TrafTranslation*TrafRotation;
}
//------------------------------------------------------------------------------
void HomoTrafo3D::set_rotation_component(const Rotation3D R) {
    if(R.uses_xyz_angels())
        set_rotation_component_based_on_xyz_angles(R);
    else
        set_rotation_component_based_on_rot_axis(R);
}
//------------------------------------------------------------------------------
void HomoTrafo3D::set_translation_component(const Vector3D &t) {
    T[0][3] = t.x();
    T[1][3] = t.y();
    T[2][3] = t.z();
}
//------------------------------------------------------------------------------
void HomoTrafo3D::set_rotation_component_based_on_rot_axis(
    const Rotation3D R
) {
        // ensure rot_axis is a unit vector
        Vector3D rot_axis = R.get_rot_axis();
        rot_axis = rot_axis/rot_axis.norm();
        
        const double rx = rot_axis.x();
        const double ry = rot_axis.y();
        const double rz = rot_axis.z();     
        
        const double sinR = sin( R.get_rot_angle_in_rad() );
        const double cosR = cos( R.get_rot_angle_in_rad() );

        // first row
        T[0][0] = cosR +  rx*rx*(1.0-cosR);
        T[0][1] = rx*ry*(1.0-cosR)-rz*sinR;
        T[0][2] = rx*rz*(1.0-cosR)+ry*sinR;
        // second row
        T[1][0] = ry*rx*(1.0-cosR)+rz*sinR;
        T[1][1] = cosR +  ry*ry*(1.0-cosR);
        T[1][2] = ry*rz*(1.0-cosR)-rx*sinR;
        // third row
        T[2][0] = rz*rx*(1.0-cosR)-ry*sinR;
        T[2][1] = rz*ry*(1.0-cosR)+rx*sinR;
        T[2][2] = cosR +  rz*rz*(1.0-cosR);
}
//------------------------------------------------------------------------------
void HomoTrafo3D::set_rotation_component_based_on_xyz_angles(
    const Rotation3D R
) {

    const double cosRx = R.cosRx();
    const double cosRy = R.cosRy();
    const double cosRz = R.cosRz();
    const double sinRx = R.sinRx();
    const double sinRy = R.sinRy();
    const double sinRz = R.sinRz();
    // first row
    T[0][0] = cosRy*cosRz;
    T[0][1] = cosRx*sinRz + sinRx*sinRy*cosRz;
    T[0][2] = sinRx*sinRz - cosRx*sinRy*cosRz;
    // second row
    T[1][0] =-cosRy*sinRz;
    T[1][1] = cosRx*cosRz - sinRx*sinRy*sinRz;
    T[1][2] = sinRx*cosRz + cosRx*sinRy*sinRz;
    // third row
    T[2][0] = sinRy;
    T[2][1] =-sinRx*cosRy;
    T[2][2] = cosRx*cosRy;
}
//------------------------------------------------------------------------------
void HomoTrafo3D::set_transformation(
    Vector3D rot_x,
    Vector3D rot_y,
    Vector3D rot_z,
    const Vector3D pos
) { 
    rot_x = rot_x/rot_x.norm();
    rot_y = rot_y/rot_y.norm();
    rot_z = rot_z/rot_z.norm();
        
    HomoTrafo3D TrafRotation;
    TrafRotation.set_x_column_of_rotation_component(rot_x);
    TrafRotation.set_y_column_of_rotation_component(rot_y);
    TrafRotation.set_z_column_of_rotation_component(rot_z);     
        
    HomoTrafo3D TrafTranslation;    
    TrafTranslation.set_translation_component(pos);

    // composition
    *this = TrafTranslation*TrafRotation;
}
//------------------------------------------------------------------------------
void HomoTrafo3D::set_x_column_of_rotation_component(const Vector3D &R) {
    T[0][0] = R.x();
    T[1][0] = R.y();
    T[2][0] = R.z();
}
//------------------------------------------------------------------------------
void HomoTrafo3D::set_y_column_of_rotation_component(const Vector3D &R) {
    T[0][1] = R.x();
    T[1][1] = R.y();
    T[2][1] = R.z();    
}
//------------------------------------------------------------------------------
void HomoTrafo3D::set_z_column_of_rotation_component(const Vector3D &R) {
    T[0][2] = R.x();
    T[1][2] = R.y();
    T[2][2] = R.z();    
}
//------------------------------------------------------------------------------
void HomoTrafo3D::transform_orientation(Vector3D* vector)const{
    vector->set(
        //x
        vector->x()*T[0][0] + 
        vector->y()*T[0][1] +
        vector->z()*T[0][2],
        //y
        vector->x()*T[1][0] + 
        vector->y()*T[1][1] + 
        vector->z()*T[1][2],
        //z
        vector->x()*T[2][0] + 
        vector->y()*T[2][1] + 
        vector->z()*T[2][2]
    );
}
//------------------------------------------------------------------------------
Vector3D HomoTrafo3D::get_transformed_orientation(const Vector3D& orientation)const {
    Vector3D transformed_orientation = orientation;
    transform_orientation(&transformed_orientation);
    return transformed_orientation; 
}
//------------------------------------------------------------------------------
void HomoTrafo3D::transform_position(Vector3D* vector)const {
    vector->set(
        //x
        vector->x()*T[0][0] + 
        vector->y()*T[0][1] + 
        vector->z()*T[0][2] + T[0][3],
        //y
        vector->x()*T[1][0] + 
        vector->y()*T[1][1] + 
        vector->z()*T[1][2] + T[1][3],
        //z
        vector->x()*T[2][0] + 
        vector->y()*T[2][1] + 
        vector->z()*T[2][2] + T[2][3]
    );
}
//------------------------------------------------------------------------------
Vector3D HomoTrafo3D::get_transformed_position(const Vector3D& pos)const {
    Vector3D transformed_position = pos;
    transform_position(&transformed_position);
    return transformed_position;
}
//------------------------------------------------------------------------------
Vector3D HomoTrafo3D::get_translation()const {
    Vector3D translation_component;

    translation_component.set(
        T[0][3], 
        T[1][3], 
        T[2][3]
    );

    return translation_component;
}
//------------------------------------------------------------------------------
std::string HomoTrafo3D::get_print()const {
    std::stringstream out; 
    out << std::setprecision(3);
    out << get_single_row_print(0);
    out << get_single_row_print(1);
    out << get_single_row_print(2);
    out << "[  0\t0\t0\t1 ]\n";
    return  out.str();
}
//------------------------------------------------------------------------------
std::string HomoTrafo3D::get_single_row_print(const uint r)const {
    std::stringstream out; 
    out << std::setprecision(3) << "[  ";
    out << T[r][0] << " \t" << T[r][1] << " \t" << T[r][2] << " \t" << T[r][3];
    out <<"  ]\n";
    return out.str();
}
//------------------------------------------------------------------------------
void HomoTrafo3D::operator= (const HomoTrafo3D G){
    for(uint row=0; row<3; row++)
        for(uint col=0; col<4; col++)
            T[row][col] = G.T[row][col];
}
//------------------------------------------------------------------------------
HomoTrafo3D HomoTrafo3D::operator* (const HomoTrafo3D G)const {
    // Matrix multiplication 
    HomoTrafo3D  M;

    // [ 0,0    0,1     0,2     0,3 ]
    // [ 1,0    1,1     1,2     1,3 ]
    // [ 2,0    2,1     2,2     2,3 ]
    // [ 3,0    3,1     3,2     3,3 ] = always [ 0.0   0.0   0.0   1.0 ]

    M.T[0][0] = T[0][0]*G.T[0][0] + T[0][1]*G.T[1][0] + T[0][2]*G.T[2][0];// + T[0][3]*      0.0
    M.T[0][1] = T[0][0]*G.T[0][1] + T[0][1]*G.T[1][1] + T[0][2]*G.T[2][1];// + T[0][3]*      0.0
    M.T[0][2] = T[0][0]*G.T[0][2] + T[0][1]*G.T[1][2] + T[0][2]*G.T[2][2];// + T[0][3]*      0.0
    M.T[0][3] = T[0][0]*G.T[0][3] + T[0][1]*G.T[1][3] + T[0][2]*G.T[2][3] + T[0][3];//*      1.0   

    M.T[1][0] = T[1][0]*G.T[0][0] + T[1][1]*G.T[1][0] + T[1][2]*G.T[2][0];// + T[1][3]*      0.0
    M.T[1][1] = T[1][0]*G.T[0][1] + T[1][1]*G.T[1][1] + T[1][2]*G.T[2][1];// + T[1][3]*      0.0
    M.T[1][2] = T[1][0]*G.T[0][2] + T[1][1]*G.T[1][2] + T[1][2]*G.T[2][2];// + T[1][3]*      0.0
    M.T[1][3] = T[1][0]*G.T[0][3] + T[1][1]*G.T[1][3] + T[1][2]*G.T[2][3] + T[1][3];//*      1.0

    M.T[2][0] = T[2][0]*G.T[0][0] + T[2][1]*G.T[1][0] + T[2][2]*G.T[2][0];// + T[2][3]*      0.0
    M.T[2][1] = T[2][0]*G.T[0][1] + T[2][1]*G.T[1][1] + T[2][2]*G.T[2][1];// + T[2][3]*      0.0
    M.T[2][2] = T[2][0]*G.T[0][2] + T[2][1]*G.T[1][2] + T[2][2]*G.T[2][2];// + T[2][3]*      0.0
    M.T[2][3] = T[2][0]*G.T[0][3] + T[2][1]*G.T[1][3] + T[2][2]*G.T[2][3] + T[2][3];//*      1.0

  //M.T[3][0] = T[3][0]*G.T[0][0] + T[3][1]*G.T[1][0] + T[3][2]*G.T[2][0] + T[3][3]*G.T[3][0];
  //M.T[3][1] = T[3][0]*G.T[0][1] + T[3][1]*G.T[1][1] + T[3][2]*G.T[2][1] + T[3][3]*G.T[3][1];
  //M.T[3][2] = T[3][0]*G.T[0][2] + T[3][1]*G.T[1][2] + T[3][2]*G.T[2][2] + T[3][3]*G.T[3][2];
  //M.T[3][3] = T[3][0]*G.T[0][3] + T[3][1]*G.T[1][3] + T[3][2]*G.T[2][3] + T[3][3]*G.T[3][3];

    return M;
}
//------------------------------------------------------------------------------
HomoTrafo3D HomoTrafo3D::inverse()const {
    HomoTrafo3D I_Rot;
    I_Rot.copy_inverse_rotation_component_from(this);

    HomoTrafo3D I_tra;
    I_tra.copy_inverse_translation_component_from(this);

    // composition
    return I_Rot*I_tra;
}
//------------------------------------------------------------------------------
void HomoTrafo3D::copy_inverse_rotation_component_from(const HomoTrafo3D *M) {
    // Transpose rot matrix because rot maticies are orthogonal
    // and therefore rot^(-1) = rot^T
    // first row of Rot matrix
    T[0][0] = M->T[0][0];
    T[0][1] = M->T[1][0];
    T[0][2] = M->T[2][0];
    // second row of Rot matrix
    T[1][0] = M->T[0][1];
    T[1][1] = M->T[1][1];
    T[1][2] = M->T[2][1];
    // third row of Rot matrix
    T[2][0] = M->T[0][2];
    T[2][1] = M->T[1][2];
    T[2][2] = M->T[2][2];
}
//------------------------------------------------------------------------------
void HomoTrafo3D::copy_inverse_translation_component_from(const HomoTrafo3D *M){
    // flip sign of translation vector to inverse the effect of the 
    // translation component
    T[0][3] = -M->T[0][3];
    T[1][3] = -M->T[1][3];
    T[2][3] = -M->T[2][3];  
}
//------------------------------------------------------------------------------
bool HomoTrafo3D::operator== (HomoTrafo3D G)const {
    for(uint row=0; row<4; row++)
        for(uint col=0; col<3; col++)
            if(T[col][row] != G.T[col][row])
                return false;

    return true;
}