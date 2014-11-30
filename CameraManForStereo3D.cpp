#include "CameraManForStereo3D.h"

CameraManForStereo3D::CameraManForStereo3D(CameraDevice* camera_to_work_with){
	camera = camera_to_work_with;
}

void CameraManForStereo3D::aquire_stereo_image(
	const CartesianFrame* world,
	const GlobalSettings* settings
){
	prepare_geometric_stereo_set_up(world, settings);
	acquire_left_and_right_image(world, settings);
	restore_initial_camera_set_up();
}

void CameraManForStereo3D::prepare_geometric_stereo_set_up(
	const CartesianFrame* world,
	const GlobalSettings* settings
){
	remember_initial_camera_position_and_rotation();
	remmber_initial_camera_image_upward_direction();
	set_positions_for_left_and_right_stereo_config();
	set_object_distance_to_focus_on(world, settings);
	set_intersec_point_for_left_and_right_optical_axis();
	set_pointing_dir_for_left_and_right_stereo_config();	
}

void CameraManForStereo3D::acquire_left_and_right_image(
	const CartesianFrame* world,
	const GlobalSettings* settings
){
	set_up_camera_in_left_stereo_config();
	camera->acquire_image(world, settings);
	camera->get_image().copyTo(left_image);	

	set_up_camera_in_right_stereo_config();
	camera->acquire_image(world, settings);
	camera->get_image().copyTo(right_image);	
}

void CameraManForStereo3D::remember_initial_camera_position_and_rotation(){
	initial_camera_pos = camera->get_position_in_world();
	initial_camera_rotation = camera->get_rotation_in_world();
}

void CameraManForStereo3D::set_positions_for_left_and_right_stereo_config(){
	left_camera_pos = initial_camera_pos - offset_to_the_right();
	right_camera_pos = initial_camera_pos + offset_to_the_right();
}

Vector3D CameraManForStereo3D::offset_to_the_right()const {
	return 	camera->direction_to_the_right_of_the_camera()*
		stereo_offset_in_m/2.0;
}

void CameraManForStereo3D::remmber_initial_camera_image_upward_direction(){
	initial_camera_image_upward_direction = 
		camera->get_image_upwards_direction_in_world_frame();
}

void CameraManForStereo3D::set_object_distance_to_focus_on(
	const CartesianFrame* world,
	const GlobalSettings* settings
){
	distance_to_object = camera->get_optical_axis_in_world().
		get_distance_to_closest_object(world,settings);

	if(distance_to_object == 0.0)
		distance_to_object = 1e2;
}

void CameraManForStereo3D::set_intersec_point_for_left_and_right_optical_axis(){
	intersection_point_for_l_and_r_optical_axes = 
		camera->get_optical_axis_in_world().PositionOnRay(distance_to_object);
	std::cout << "object distance: "<<distance_to_object<<"\n";
}

void CameraManForStereo3D::set_pointing_dir_for_left_and_right_stereo_config(){
	left_camera_direction_optical_axis = 
		intersection_point_for_l_and_r_optical_axes - left_camera_pos;

	right_camera_direction_optical_axis = 
		intersection_point_for_l_and_r_optical_axes - right_camera_pos;

	left_camera_direction_optical_axis.normalize();	
	right_camera_direction_optical_axis.normalize();
}

void CameraManForStereo3D::set_up_camera_in_left_stereo_config(){
	camera->update_position(left_camera_pos);
	camera->set_pointing_direction(
		left_camera_direction_optical_axis,
		initial_camera_image_upward_direction
	);
}

void CameraManForStereo3D::set_up_camera_in_right_stereo_config(){
	camera->update_position(right_camera_pos);
	camera->set_pointing_direction(
		right_camera_direction_optical_axis,
		initial_camera_image_upward_direction
	);	
}

void CameraManForStereo3D::restore_initial_camera_set_up(){
	camera->update_position_and_orientation(
		initial_camera_pos,
		initial_camera_rotation
	);
}

void CameraManForStereo3D::increase_stereo_offset(){
	stereo_offset_in_m = stereo_offset_in_m * 1.05;
	print_stereo_offset_manipulation("increased to");
}

void CameraManForStereo3D::decrease_stereo_offset(){
	stereo_offset_in_m = stereo_offset_in_m / 1.05;
	print_stereo_offset_manipulation("decreased to");
}

void CameraManForStereo3D::print_stereo_offset_manipulation(
	const std::string status
)const{
	std::cout << camera->get_name() << " stereo offset " << status << ": ";
	std::cout << stereo_offset_in_m << "m\n";
}

cv::Mat CameraManForStereo3D::get_anaglyph_stereo3D_image() {

	CameraImage stereo_image(
		left_image.cols,
		left_image.rows
	);	

	// BGR
	std::vector<cv::Mat> BGR_left(3);
	cv::split(left_image, BGR_left);
	
	std::vector<cv::Mat> BGR_right(3);
	cv::split(right_image, BGR_right);
	
	std::vector<cv::Mat> anaglyph_image_channels;		
	
	// 0 -> B 
	anaglyph_image_channels.push_back(BGR_right.at(0));
	// 1 -> G 
	anaglyph_image_channels.push_back(BGR_right.at(1));
	// 2 -> R 
	anaglyph_image_channels.push_back(BGR_left.at(2) );
	
	cv::merge(anaglyph_image_channels, stereo_image.get_image());

	return stereo_image.get_image();
}