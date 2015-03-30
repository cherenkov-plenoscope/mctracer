#include "ApertureCamera.h"
//------------------------------------------------------------------------------
void ApertureCamera::set_aperture_cam(	
	const double new_FStopNumber,
	const double new_SensorSizeX,
	const uint rays_per_pixel 
){
	set_F_stop_number(new_FStopNumber);
	set_sensor_size_using_width(new_SensorSizeX);
	update_sensor_pixel_pitch();
	set_number_of_rays_per_pixel(rays_per_pixel);

	set_default_object_distance();
	update_sensor_distance_given_focal_and_object_distance();

	// init back transformation T_cam2world
	T_World2Camera = T_Camera2World.inverse();
}
//------------------------------------------------------------------------------
void ApertureCamera::set_F_stop_number(const double new_FStopNumber){
	assert_F_number_is_positive(new_FStopNumber);
	FStopNumber = new_FStopNumber;
}
//------------------------------------------------------------------------------
void ApertureCamera::set_sensor_size_using_width(const double width_in_m){
	assert_sensor_width_is_positive(width_in_m);
	sensor_width_in_m = width_in_m;
	sensor_height_in_m = sensor_width_in_m / image->get_width_to_height_ratio();
}
//------------------------------------------------------------------------------
void ApertureCamera::update_sensor_pixel_pitch(){
	PixelPitch_in_m = sensor_width_in_m/double(image->get_number_of_cols());
}
//------------------------------------------------------------------------------
void ApertureCamera::set_number_of_rays_per_pixel(const uint rays_per_pixel){
	assert_number_of_rays_per_pixel_is_positive(rays_per_pixel);
	this->rays_per_pixel = rays_per_pixel;	
}
//------------------------------------------------------------------------------
void ApertureCamera::set_default_object_distance(){
	set_object_distance(default_object_distance_in_m);
}
//------------------------------------------------------------------------------
void ApertureCamera::set_object_distance(const double ObjectDistance_in_m){
	assert_object_distance_is_positive(ObjectDistance_in_m);
	this->ObjectDistance_in_m = ObjectDistance_in_m;
}
//------------------------------------------------------------------------------
void ApertureCamera::update_sensor_distance_given_focal_and_object_distance(){
	// 1/f = 1/b + 1/g
	// f = FocalLength_in_m
	// b = SensorDistance_in_m
	// g = ObjectDistance_in_m
	SensorDistance_in_m = 1.0/(1.0/FocalLength_in_m - 1.0/ObjectDistance_in_m);
}
//------------------------------------------------------------------------------
void ApertureCamera::set_focus_to(const double ObjectDistance_in_m){
	set_object_distance(ObjectDistance_in_m);
	update_sensor_distance_given_focal_and_object_distance();
}
//------------------------------------------------------------------------------
void ApertureCamera::set_FoV_in_rad(const double FoV_in_rad) {
	assert_FoV_is_valid(FoV_in_rad);
	this -> FoV_in_rad = FoV_in_rad;
	update_focal_length();
}
//------------------------------------------------------------------------------
void ApertureCamera::update_focal_length() {
	
	FocalLength_in_m = (sensor_width_in_m/2.0)/tan(FoV_in_rad/2.0);
	update_aperture_radius();
	update_sensor_distance_given_focal_and_object_distance();
}
//------------------------------------------------------------------------------
void ApertureCamera::update_aperture_radius() {
	ApertureRadius_in_m = FocalLength_in_m/(2.0*FStopNumber);
}
//------------------------------------------------------------------------------
void ApertureCamera::auto_focus(
	const CartesianFrame* world,
	const GlobalSettings* settings
){
	set_focus_to(get_average_object_distance(world, settings));
}
//------------------------------------------------------------------------------
double ApertureCamera::get_average_object_distance(
	const CartesianFrame* world,
	const GlobalSettings* settings
) {
	double sum_of_valid_object_distances = 0.0;
	uint number_of_valid_distances = 0;
	const uint fraction_of_image_pixels = image->get_resolution()*5e-4;

	for(uint pixel_it=0; pixel_it < fraction_of_image_pixels; pixel_it++) {

		CameraRay ray = get_ray_for_pixel_in_row_and_col(
			get_random_row(),
			get_random_col()
		);

		DistanceMeter dist_meter(&ray, world);

		if(dist_meter.does_face_an_object()) {
			number_of_valid_distances++;
			sum_of_valid_object_distances += 
				dist_meter.get_distance_to_closest_object();
		}
	}

	if(number_of_valid_distances == 0){
		return default_object_distance_in_m;
	}else{
		return sum_of_valid_object_distances/double(number_of_valid_distances);
	}
}
//------------------------------------------------------------------------------
uint ApertureCamera::get_random_row(){
	return uint(floor(dice.uniform()*image->get_number_of_rows()));
}
//------------------------------------------------------------------------------
uint ApertureCamera::get_random_col(){
	return uint(floor(dice.uniform()*image->get_number_of_cols()));
}
//------------------------------------------------------------------------------
void ApertureCamera::assert_object_distance_is_positive(
	const double ObjectDistance_in_m
)const{
	if(ObjectDistance_in_m <= 0.0) {
		std::stringstream out;
		out << "ApertureCamera::" << __func__ << "()\n";
		out << "Object distance g must not be negative! ";
		out << "Expected g > 0.0m, but actual: ";
		out << ObjectDistance_in_m << "m\n";
		throw TracerException(out.str());
	}
}
//------------------------------------------------------------------------------
void ApertureCamera::assert_number_of_rays_per_pixel_is_positive(
	const uint rays_per_pixel
)const{
	if(rays_per_pixel <= 0){
		std::stringstream out;
		out << "ApertureCamera::" << __func__ << "()\n";
		out << "The number N of rays emitted per pixel must not be negative!";
		out << "Expected:N > 0, but actual: " << rays_per_pixel << std::endl;
		throw TracerException(out.str());
	}		
}
//------------------------------------------------------------------------------
void ApertureCamera::assert_F_number_is_positive(
	const double FStopNumber
)const{
	if(FStopNumber <= 0.0){
		std::stringstream out;
		out << "ApertureCamera::" << __func__ << "()\n";
		out << "Focal ratio / F-stop number must not be negative!";
		out << "Expected: F-stop > 0.0, but actual: ";
		out << FStopNumber << std::endl;
		throw TracerException(out.str());
	}	
}
//------------------------------------------------------------------------------
void ApertureCamera::assert_sensor_width_is_positive(
	const double sensor_width_in_m
)const{
	if(sensor_width_in_m <= 0.0){
		std::stringstream out;
		out << "ApertureCamera::" << __func__ << "()\n";
		out << "Sensor width must not be negative!";
		out << "Expected: width > 0.0, but actual: ";
		out << sensor_width_in_m << std::endl;
		throw TracerException(out.str());
	}		
}
//------------------------------------------------------------------------------
std::string ApertureCamera::get_aperture_camera_print()const {
	std::stringstream out;
	out << "| Focal ratio      : " << FStopNumber << "\n";	
	out << "| Focal length     : " << FocalLength_in_m*1e3 << " mm\n";	
	out << "| Aperture diameter: " << ApertureRadius_in_m*2e3 << " mm\n";	
	out << "| Sensor size      : ";
	out << 1e3*sensor_width_in_m << " x " << 1e3*sensor_height_in_m << " mm^2\n";	
	out << "| Pixel pitch      : " << PixelPitch_in_m*1e6 << " um\n";
	out << "| Object distance  : " << ObjectDistance_in_m << " m\n";
	out << "| Sensor distance  : " << SensorDistance_in_m*1e3 << " mm\n";
	out << "| Rays per pixel   : " << rays_per_pixel << "\n";
	out << "| Rays per image   : " << 
	double(rays_per_pixel *	image->get_resolution()/1e6) << " M rays\n";
	return out.str();
}
//------------------------------------------------------------------------------
void ApertureCamera::print()const{
	std::cout << get_camera_print();
	std::cout << get_aperture_camera_print();
}
//------------------------------------------------------------------------------
Vector3D ApertureCamera::get_random_point_on_bounded_aperture_plane() {

	double polar_radius = dice.uniform()*ApertureRadius_in_m;
	double polar_angel = dice.uniform()*2.0*M_PI;

	return Vector3D(
		polar_radius*cos(polar_angel), 
		polar_radius*sin(polar_angel), 
		SensorDistance_in_m
	);	
}
//------------------------------------------------------------------------------
Vector3D ApertureCamera::get_intersec_of_cam_ray_for_pix_row_col_with_obj_plane(
	const uint row, const uint col
){
	const int x_pos_on_sensor_in_pixel =  row - image->get_number_of_rows()/2;
	const int y_pos_on_sensor_in_pixel =  col - image->get_number_of_cols()/2;
 
 	const double image_size_x = x_pos_on_sensor_in_pixel * PixelPitch_in_m;
	const double image_size_y = y_pos_on_sensor_in_pixel * PixelPitch_in_m;

	return Vector3D(
		get_object_size_for_image_size(image_size_x),
		get_object_size_for_image_size(image_size_y), 
		ObjectDistance_in_m
	);
}
//------------------------------------------------------------------------------
double ApertureCamera::get_object_size_for_image_size(
	const double image_size_in_m
)const{
	return image_size_in_m * ObjectDistance_in_m / SensorDistance_in_m;
}
//------------------------------------------------------------------------------
CameraRay ApertureCamera::get_ray_for_pixel_in_row_and_col(
	const uint row, const uint col
){	
	Vector3D support_vec_in_cam_frame = 
		get_random_point_on_bounded_aperture_plane();
	
	Vector3D obj_plane_intersec_in_cam_frame = 
		get_intersec_of_cam_ray_for_pix_row_col_with_obj_plane(row, col);
	
	Vector3D direction_vec_in_cam_frame =
		obj_plane_intersec_in_cam_frame - support_vec_in_cam_frame;
	
	return CameraRay(
		camera_ray_support_vector_in_world_frame(support_vec_in_cam_frame),
		camera_ray_direction_vector_in_world_frame(direction_vec_in_cam_frame)
	);
}
//------------------------------------------------------------------------------
Vector3D ApertureCamera::camera_ray_support_vector_in_world_frame(
	Vector3D &cam_ray_support_vec_in_cam_frame
)const{
	T_Camera2World.transform_orientation(&cam_ray_support_vec_in_cam_frame);
	return CameraPositionInWorld + cam_ray_support_vec_in_cam_frame;
}
//------------------------------------------------------------------------------
Vector3D ApertureCamera::camera_ray_direction_vector_in_world_frame(
	Vector3D &cam_ray_direction_vec
)const{
	T_Camera2World.transform_orientation(&cam_ray_direction_vec);
	return cam_ray_direction_vec;
}
//------------------------------------------------------------------------------
void ApertureCamera::acquire_image(
	const CartesianFrame* world,
	const GlobalSettings* settings
){
	double color_weight_for_single_ray = 
		1.0/double(rays_per_pixel);
	
	CameraRay cam_ray;
	Color average_pixel_color;
	Color color_for_single_ray;
	uint pixel_iterator, row, col;
	const uint initial_interaction_counter = 0;
	
	#pragma omp parallel shared(settings,world) private(pixel_iterator,cam_ray,average_pixel_color,color_for_single_ray,row,col) 
	{	
		#pragma omp for schedule(dynamic) 
		for (pixel_iterator = 0; pixel_iterator < image->get_resolution(); pixel_iterator++) 
		{
			row = pixel_iterator / image->get_number_of_cols();
			col = pixel_iterator % image->get_number_of_cols();
			average_pixel_color.set_RGB_0to255(0,0,0);

			for(int j = 0; j < rays_per_pixel; j++ ){

				cam_ray = get_ray_for_pixel_in_row_and_col(row, col);
			
				color_for_single_ray = cam_ray.trace(
					world,
					initial_interaction_counter,
					settings
				);
				
				average_pixel_color.mixture(
					&color_for_single_ray,
					color_weight_for_single_ray
				);
			}
			image->set_pixel_row_col_to_color(row, col, average_pixel_color);
		}
	}  
}
//------------------------------------------------------------------------------