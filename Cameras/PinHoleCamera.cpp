#include "PinHoleCamera.h"
//------------------------------------------------------------------------------
void PinHoleCamera::update_position_and_orientation(		
	const Vector3D cam_pos_in_world,
	const Rotation3D cam_rot_in_world
){
	set_position_and_orientation(cam_pos_in_world, cam_rot_in_world);
	update_optical_axis_and_orientation();

	update_principal_point_for_current_FoV();

	// calculate sensor vectors
	SensorDirectionHori = Vector3D::unit_y;
	SensorDirectionVert = Vector3D::unit_x;
	T_Camera2World.transform_orientation(&SensorDirectionHori);
	T_Camera2World.transform_orientation(&SensorDirectionVert);
}
//------------------------------------------------------------------------------
void PinHoleCamera::set_FoV_in_rad(const double FoV_in_rad) {
	assert_FoV_is_valid(FoV_in_rad);
	this->FoV_in_rad = FoV_in_rad;
	update_principal_point_for_current_FoV();
}
//------------------------------------------------------------------------------
void PinHoleCamera::update_principal_point_for_current_FoV(){
	/*
	 calculate principal point (intersection of optical axis and
	 image sensor plane)
	 calculate distance of camera base and principal point
	 using FoV_in_rad angle 
	
		         camera base
		              /|  \
		             / |  |
		            /  |  |
		           /   |  |
		          /    |  |
		         /fov/2|  |
		        /______|  |
		       /       |   \
		      /        |   /dist_camera_support_to_principal_point
		     /         |  |
		    /          |  |
		   /       ____|  |
		  /       | o  |  |
		 /________|____|  /
		        principal_point
		\______  ______/
		       \/
		      row/2
	*/	
	//distance
	dist_camera_support_to_principal_point =
	((image->get_number_of_cols()/2.0)/tan(FoV_in_rad/2.0));

	// principal point
	principal_point = 
	CameraPointingDirection*dist_camera_support_to_principal_point;		
}
//------------------------------------------------------------------------------
std::string PinHoleCamera::get_pin_hole_cam_print()const {
	std::stringstream out;
	out << get_camera_print();
	out << "| distance camera support to principal point: ";
	out << dist_camera_support_to_principal_point << "m\n";
	return out.str();
}
//------------------------------------------------------------------------------
void PinHoleCamera::print()const {
	std::cout << get_pin_hole_cam_print();
}
//------------------------------------------------------------------------------
CameraRay PinHoleCamera::get_ray_for_pixel_in_row_and_col(
	const int row, const int col
)const{
	return CameraRay(
		CameraPositionInWorld, get_direction_of_ray_for_pixel(row, col)
	);
}
//------------------------------------------------------------------------------
Vector3D PinHoleCamera::get_direction_of_ray_for_pixel(
	const int row, const int col
)const {
	return get_intersection_of_ray_on_image_sensor_for_pixel(row,col) - 
		CameraPositionInWorld;
}
//------------------------------------------------------------------------------
Vector3D PinHoleCamera::get_intersection_of_ray_on_image_sensor_for_pixel(
	const int row, const int col
)const{
	const int vert_position_on_image_sensor = row-image->get_number_of_rows()/2;
	const int hori_position_on_image_sensor = col-image->get_number_of_cols()/2;

	return principal_point + 
		SensorDirectionVert * vert_position_on_image_sensor + 
		SensorDirectionHori * hori_position_on_image_sensor;
}
//------------------------------------------------------------------------------
void PinHoleCamera::acquire_image(	
	const CartesianFrame* world, const GlobalSettings* settings
){
	uint i, row, col;
	CameraRay cam_ray;
	Color color;

	#pragma omp parallel shared(settings,world) private(i,cam_ray,color,row,col) 
	{	
		#pragma omp for schedule(dynamic) 
		for (i = 0; i < image->get_resolution(); i++) {
			
			row = i / image->get_number_of_cols();
			col = i % image->get_number_of_cols();

			cam_ray = get_ray_for_pixel_in_row_and_col(row, col);
			color = cam_ray.trace(world, 0, settings);

			image->set_pixel_row_col_to_color(row, col, color);
		}
	}
}