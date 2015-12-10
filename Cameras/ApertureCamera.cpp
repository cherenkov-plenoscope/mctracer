#include "ApertureCamera.h"
#include "Tools/AssertionTools.h"
//------------------------------------------------------------------------------
void ApertureCamera::set_fStop_sesnorWidth_rayPerPixel(	
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
	AssertionTools::value_with_name_is_greater_zero_given_context(
		new_FStopNumber, "F-Stop number of aerture", 
		"The F-Stop number, i.e. the f over d ratio must be positiv non zero."
	);
	FStopNumber = new_FStopNumber;
}
//------------------------------------------------------------------------------
void ApertureCamera::set_sensor_size_using_width(const double width_in_m) {
	AssertionTools::value_with_name_is_greater_zero_given_context(
		width_in_m, "image sensor width", 
		"The image sensor width of the aperture camera must be positiv non zero"
	);
	sensor_width_in_m = width_in_m;
	sensor_height_in_m = sensor_width_in_m / image->get_width_to_height_ratio();
}
//------------------------------------------------------------------------------
void ApertureCamera::update_sensor_pixel_pitch(){
	PixelPitch_in_m = sensor_width_in_m/double(image->get_number_of_cols());
}
//------------------------------------------------------------------------------
void ApertureCamera::set_number_of_rays_per_pixel(const uint rays_per_pixel) {
	AssertionTools::value_with_name_is_greater_zero_given_context(
		rays_per_pixel, "number of rays emitted for each pixel",
		"The number of rays emiited must be non zero and positiv."
	);
	this->rays_per_pixel = rays_per_pixel;	
}
//------------------------------------------------------------------------------
void ApertureCamera::set_default_object_distance(){
	set_object_distance(default_object_distance_in_m);
}
//------------------------------------------------------------------------------
void ApertureCamera::set_object_distance(const double ObjectDistance_in_m){
	AssertionTools::value_with_name_is_greater_zero_given_context(
		ObjectDistance_in_m, "object_distance", 
		"The object distance must be greater zero for objects to be seen by "
		"the aperture camera."
	);
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
	const Frame* world,
	const TracerSettings* settings
){
	set_focus_to(get_average_object_distance(world, settings));
}
//------------------------------------------------------------------------------
uint ApertureCamera::_5_permil_of_pixels()const {
	return image->get_number_of_pixels()*5e-4;
}
//------------------------------------------------------------------------------
double ApertureCamera::get_average_object_distance(
	const Frame* world,
	const TracerSettings* settings
) {
	double sum_of_valid_object_distances = 0.0;
	uint number_of_valid_distances = 0;

	for(uint pixel_it=0; pixel_it < _5_permil_of_pixels(); pixel_it++) {

		CameraRay ray = get_ray_for_pixel_in_row_and_col(
			get_random_row(),
			get_random_col()
		);

		DistanceMeter dist_meter(&ray, world);

		if(dist_meter.does_face_an_object()) {
			number_of_valid_distances++;
			sum_of_valid_object_distances = sum_of_valid_object_distances + 
				dist_meter.get_distance_to_closest_object();
		}
	}

	if(number_of_valid_distances == 0)
		return default_object_distance_in_m;
	else
		return sum_of_valid_object_distances/double(number_of_valid_distances);
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
	double(rays_per_pixel *	image->get_number_of_pixels()/1e6) << " M rays\n";
	return out.str();
}
//------------------------------------------------------------------------------
std::string ApertureCamera::get_print()const {
	std::stringstream out;
	out << get_camera_print() << get_aperture_camera_print();
	return out.str();
}
//------------------------------------------------------------------------------
Vector3D ApertureCamera::get_random_point_on_bounded_aperture_plane() {

	Vector3D ap = dice.get_point_on_xy_disc_within_radius(ApertureRadius_in_m);
	return Vector3D(ap.x(), ap.y(), SensorDistance_in_m);
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
	const Vector3D support_vec_in_cam_frame = 
		get_random_point_on_bounded_aperture_plane();
	
	const Vector3D obj_plane_intersec_in_cam_frame = 
		get_intersec_of_cam_ray_for_pix_row_col_with_obj_plane(row, col);
	
	const Vector3D direction_vec_in_cam_frame =
		obj_plane_intersec_in_cam_frame - support_vec_in_cam_frame;
	
	return CameraRay(
		camera_ray_support_vector_in_world_frame(support_vec_in_cam_frame),
		camera_ray_direction_vector_in_world_frame(direction_vec_in_cam_frame)
	);
}
//------------------------------------------------------------------------------
Vector3D ApertureCamera::camera_ray_support_vector_in_world_frame(
	const Vector3D &cam_ray_support_in_cam_frame
)const{
	return CameraPositionInWorld + T_Camera2World.get_transformed_orientation(
		cam_ray_support_in_cam_frame
	);
}
//------------------------------------------------------------------------------
Vector3D ApertureCamera::camera_ray_direction_vector_in_world_frame(
	const Vector3D &cam_ray_direction
)const{
	return T_Camera2World.get_transformed_orientation(cam_ray_direction);
}
//------------------------------------------------------------------------------
void ApertureCamera::acquire_image(
	const Frame* world,
	const TracerSettings* settings
){	
	CameraRay cam_ray;
	uint pix, row, col;
	const uint initial_interaction_counter = 0;
	int HadCatch = 0;
	
	#pragma omp parallel shared(settings,world,HadCatch) private(pix,cam_ray,row,col) 
	{	
		#pragma omp for schedule(dynamic) 
		for (pix = 0; pix < image->get_number_of_pixels(); pix++) 
		{
			try {
				row = pix / image->get_number_of_cols();
				col = pix % image->get_number_of_cols();
				std::vector<Color> colors_of_pixel; 
				colors_of_pixel.reserve(rays_per_pixel);

				for(int j = 0; j < rays_per_pixel; j++ ){

					cam_ray = get_ray_for_pixel_in_row_and_col(row, col);
				
					colors_of_pixel.push_back(
						cam_ray.trace(
							world,
							initial_interaction_counter,
							settings
						)
					);
				}
				image->set_pixel_row_col_to_color(row, col, Color(colors_of_pixel));
			}catch(std::exception &error) {
				HadCatch++;
				std::cerr << error.what(); 
			}catch(...) {
				HadCatch++;
			}
		}
	}

	if(HadCatch) {
		std::stringstream info;
		info << "ApertureCamera::"<<__func__<<"() in "<<__FILE__<<", "<<__LINE__<<"\n";
		info << "Cought exception during multithread ray tracing.\n";
		throw(TracerException(info.str()));
	} 
}
//------------------------------------------------------------------------------