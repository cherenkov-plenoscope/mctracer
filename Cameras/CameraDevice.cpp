#include "CameraDevice.h"
//------------------------------------------------------------------------------
CameraDevice::CameraDevice(
	const std::string camera_name, 
	const uint sensor_cols, 
	const uint sensor_rows
): CameraName(camera_name){
	image = new CameraImage(sensor_cols, sensor_rows);
}
//------------------------------------------------------------------------------
void CameraDevice::update_position(const Vector3D new_cam_pos_in_world) {
	update_position_and_orientation(
		new_cam_pos_in_world, 
		CameraOrientationInWorld
	);
}
//------------------------------------------------------------------------------
void CameraDevice::update_orientation(const Rotation3D new_cam_rot_in_world) {
	update_position_and_orientation(
		CameraPositionInWorld, 
		new_cam_rot_in_world
	);
}
//------------------------------------------------------------------------------
void CameraDevice::update_position_and_orientation(		
	const Vector3D new_cam_pos_in_world,
	const Rotation3D new_cam_rot_in_world
){
	set_position_and_orientation(new_cam_pos_in_world, new_cam_rot_in_world);
	update_optical_axis_and_orientation();
}
//------------------------------------------------------------------------------
void CameraDevice::set_position_and_orientation(	
	const Vector3D cam_pos_in_world,
	const Rotation3D cam_rot_in_world
){
	this->CameraPositionInWorld = cam_pos_in_world;
	this->CameraOrientationInWorld = cam_rot_in_world;

	T_Camera2World.set_transformation(
		CameraOrientationInWorld,
		CameraPositionInWorld
	);
}
//------------------------------------------------------------------------------
void CameraDevice::update_optical_axis_and_orientation() {
	CameraPointingDirection.set_unit_vector_z();
	T_Camera2World.transform_orientation(&CameraPointingDirection);

	OpticalAxis.SetSupport(CameraPositionInWorld);
	OpticalAxis.SetDirection(CameraPointingDirection);	
}
//------------------------------------------------------------------------------
void CameraDevice::set_pointing_direction(	
	Vector3D camera_pointing_direction_in_World,
	Vector3D camera_image_upwards_image_dir_in_world
){
	camera_pointing_direction_in_World.normalize();
	
	// image orientation is defined by x,y.
	// camera y is supposed to point "upwards" in world system
	Vector3D cam_z_axis_in_world = camera_pointing_direction_in_World;
	Vector3D cam_y_axis_in_world = camera_image_upwards_image_dir_in_world;
	Vector3D cam_x_axis_in_world = 
		cam_y_axis_in_world.CrossProduct(cam_z_axis_in_world);
	
	T_Camera2World.set_transformation(
		cam_x_axis_in_world,
		cam_y_axis_in_world,
		cam_z_axis_in_world,
		CameraPositionInWorld
	);
	
	update_optical_axis_and_orientation();
}
//------------------------------------------------------------------------------
Vector3D CameraDevice::get_image_upwards_direction_in_world_frame()const{

	Vector3D image_upwards_direction_in_world_frame;
	image_upwards_direction_in_world_frame.set_unit_vector_y();
	
	T_Camera2World.transform_orientation(
		&image_upwards_direction_in_world_frame
	);
	
	return image_upwards_direction_in_world_frame;
}
//------------------------------------------------------------------------------
std::string CameraDevice::get_camera_print()const{
	std::stringstream out;
	out << " _camera:_" << CameraName << "________________\n";
	out << "| T_" << CameraName << "2world:\n";
	out << StringTools::place_first_infront_of_each_new_line_of_second(
		"| ",
		T_Camera2World.get_print()
	);	
	out << "| camera position          : " << CameraPositionInWorld << "\n";
	out << "| direction of optical axis: " << CameraPointingDirection << "\n";
	out << "| field of view: " << Rad2Deg(FoV_in_rad) <<" deg\n";
	out << "| resolution: cols x rows : ";
	out << image->get_number_of_cols() << "x";
	out << image->get_number_of_rows() <<" pixels";
	out << " / " << image->get_resolution()/1e6 << " M pixels\n";
	return out.str();
}
//------------------------------------------------------------------------------
Vector3D CameraDevice::get_normalized_pointing_direction()const{
	return CameraPointingDirection/CameraPointingDirection.norm2();
}
//------------------------------------------------------------------------------
void CameraDevice::print()const{
	std::cout << get_camera_print();
}
//------------------------------------------------------------------------------
void CameraDevice::set_FoV_in_rad(const double FoV_in_rad) {
	assert_FoV_is_valid(FoV_in_rad);
	this -> FoV_in_rad = FoV_in_rad;
}
//------------------------------------------------------------------------------
double CameraDevice::get_FoV_in_rad()const {
	return FoV_in_rad;
}
//------------------------------------------------------------------------------
void CameraDevice::assert_FoV_is_valid(const double FoV_in_rad)const {
	if( FoV_in_rad <= Deg2Rad(0.0) || FoV_in_rad > Deg2Rad(180.0) ) {
		std::stringstream info;
		info << "CameraDevice::" << __func__ << "()\n";
		info << "Expected 0.0DEG < FoV < 180.0DEG, but actual FoV: ";
		info << Rad2Deg(FoV_in_rad) << "DEG\n";
		throw TracerException(info.str());
	}
}
//------------------------------------------------------------------------------
std::string CameraDevice::get_name()const {
	return CameraName;
}
//------------------------------------------------------------------------------
const CameraImage* CameraDevice::get_image()const {
	return image;
}
//------------------------------------------------------------------------------
void CameraDevice::save_image(const std::string image_path)const {
	image->save_image_to_file(image_path);
}
//------------------------------------------------------------------------------
Vector3D CameraDevice::get_position_in_world()const{
	return CameraPositionInWorld;
}
//------------------------------------------------------------------------------
Rotation3D CameraDevice::get_rotation_in_world()const{
	return CameraOrientationInWorld;
}
//------------------------------------------------------------------------------
Vector3D CameraDevice::direction_to_the_right_of_the_camera()const{
	Vector3D ez; ez.set_unit_vector_z(); 
	return ez.CrossProduct(get_normalized_pointing_direction());	
}
Ray CameraDevice::get_optical_axis_in_world()const{
	return OpticalAxis;
}
//------------------------------------------------------------------------------
void CameraDevice::acquire_image(
	const CartesianFrame* world, const GlobalSettings* settings
){
	std::cout << "Calling " << __func__ << " in CameraDevice!\n";
}