#include "FreeOrbitCamera.h"
//------------------------------------------------------------------------------
FreeOrbitCamera::FreeOrbitCamera(
	const Frame *world,
	const TracerSettings *settings
){	
	this->world = world;
	this->settings = settings;

	flying_camera = new PinHoleCamera("Cam", 
		settings->preview.cols, 
		settings->preview.rows
	);
	
	create_CameraMen_to_safely_operate_the_flying_camera();
	reset_camera();
	time_stamp.update_now();
	
	start_free_orbit();
}
//------------------------------------------------------------------------------
void FreeOrbitCamera::create_CameraMen_to_safely_operate_the_flying_camera(){
	FoV_operator = new CameraManFoV(flying_camera);
	FoV_operator->set_verbosity(true);

	Translation_operator = new CameraManForTranslation(flying_camera);
	Translation_operator->set_verbosity(true);

	Rotation_operator = new CameraManForRotation(flying_camera);
	Rotation_operator->set_verbosity(true);

	Stereo_operator = new CameraManForStereo3D(flying_camera);
	Stereo_operator->set_verbosity(true);
}
//------------------------------------------------------------------------------
void FreeOrbitCamera::reset_camera(){
	Translation_operator->set_default_position(Vector3D(0.0, 0.0, 0.0));
	Rotation_operator->set_default_rotation(Rotation3D(0.0,Deg2Rad(-90.0),0.0));
	FoV_operator->set_default_FoV();
}
//------------------------------------------------------------------------------
void FreeOrbitCamera::start_free_orbit(){

	UserInteraction::print_welcome_screen();
	print_free_orbit_help_text();
	create_free_orbit_display();

	int user_input_key = 0;
	bool key_stroke_requires_image_update = true;

	while(!UserInteraction::is_Escape_key(user_input_key)) {

		iteration_counter++;

		if(it_is_time_again_to_show_the_help())
			print_free_orbit_help_text();

		if(key_stroke_requires_image_update)
			update_free_orbit_display();
		
		key_stroke_requires_image_update = true;
		user_input_key = wait_for_user_key_stroke();

		switch(user_input_key){
			case 't': toggle_stereo3D();
			break;
			case 'w': Translation_operator->move_for();
			break;
			case 's': Translation_operator->move_back();
			break;
			case 'a': Translation_operator->move_left();
			break;
			case 'd': Translation_operator->move_right();
			break;
			case 'q': Translation_operator->move_up();
			break;
			case 'e': Translation_operator->move_down();
			break;
			case 'n': FoV_operator->increase_FoV_when_possible();
			break;
			case 'm': FoV_operator->decrease_FoV_when_possible();
			break;
			case 'i': Rotation_operator->look_further_up_when_possible();
			break;
			case 'k': Rotation_operator->look_further_down_when_possible();
			break;
			case 'j': Rotation_operator->look_left();
			break;
			case 'l': Rotation_operator->look_right();
			break;
			case 'x': Stereo_operator->increase_stereo_offset();
			break;
			case 'y': Stereo_operator->decrease_stereo_offset();
			break;
			case 'p': {
				std::cout << world->get_tree_print() << "\n";
				key_stroke_requires_image_update = false;
			}
			break;
			case 'h': {
				print_free_orbit_help_text();
				key_stroke_requires_image_update = false;
			};
			break;
			default: key_stroke_requires_image_update = false;
		}
	}
	destroy_free_orbit_display();
}
//------------------------------------------------------------------------------
int FreeOrbitCamera::wait_for_user_key_stroke() {
	return cvWaitKey(0);
}
//------------------------------------------------------------------------------
bool FreeOrbitCamera::it_is_time_again_to_show_the_help() {
	if(iteration_counter > 15) {
		iteration_counter = 0;
		return true;
	}else{
		return false;
	}
}
//------------------------------------------------------------------------------
void FreeOrbitCamera::create_free_orbit_display(){
	cv::namedWindow( free_orbit_display_name, CV_WINDOW_AUTOSIZE );

	FreeOrbitCamera* p = this;

	cv::setMouseCallback( 
		free_orbit_display_name.c_str(), 
		mouse_button_event, 
		(void *)p 
	);
}
//------------------------------------------------------------------------------
void FreeOrbitCamera::destroy_free_orbit_display(){
	cv::destroyWindow(free_orbit_display_name);	
}
//------------------------------------------------------------------------------
void FreeOrbitCamera::update_free_orbit_display(){
	const CameraImage* img = acquire_image_with_camera(flying_camera);

	cv::imshow(
		free_orbit_display_name,
		*(img->Image)
	); 
}
//------------------------------------------------------------------------------
void FreeOrbitCamera::mouse_button_event(
	int event, int x, int y, int flags, void *param
) {
	FreeOrbitCamera* p = (FreeOrbitCamera*)param;
	
	if( event == cv::EVENT_LBUTTONDOWN )
		p->print_info_of_probing_ray_for_pixel_col_row( x, y );
	else if( event == cv::EVENT_RBUTTONDOWN )
		p->take_snapshot_manual_focus_on_pixel_col_row( x, y );
	else
		return;
}
//------------------------------------------------------------------------------
void FreeOrbitCamera::toggle_stereo3D() {
	stereo3D = !stereo3D;
	std::cout << get_prefix_print() << "Stereo 3D : ";
	std::cout << (stereo3D ? "On" : "Off") << "\n";
}
//------------------------------------------------------------------------------
std::string FreeOrbitCamera::get_snapshot_filename(){
	snapshot_counter++;

	std::stringstream filename;  
	filename << time_stamp.yyyy() << time_stamp.mm() << time_stamp.dd() << "_";
	filename << time_stamp.HH() << time_stamp.MM() << time_stamp.SS();
	filename << "_" << snapshot_counter;
	return filename.str();
}
//------------------------------------------------------------------------------
ApertureCamera FreeOrbitCamera::get_ApertureCamera_based_on_free_orbit_camera()const{

	ApertureCamera apcam("Imax70mm", 
		settings->snapshot.cols, 
		settings->snapshot.rows
	);

	apcam.set_fStop_sesnorWidth_rayPerPixel(
		0.95, 
		0.07, 
		settings->snapshot.rays_per_pixel
	);

	apcam.set_FoV_in_rad(flying_camera->get_FoV_in_rad());
	apcam.update_position_and_orientation(
		flying_camera->get_position_in_world(),
		flying_camera->get_rotation_in_world()
	);
	return apcam;
}
//------------------------------------------------------------------------------
std::string FreeOrbitCamera::get_prefix_print()const{
	return "free_orbit -> ";
}
//------------------------------------------------------------------------------
void FreeOrbitCamera::take_snapshot_manual_focus_on_pixel_col_row(int x, int y){

	Ray probing_ray = flying_camera->get_ray_for_pixel_in_row_and_col(y, x);
	
	DistanceMeter dist_meter(&probing_ray, world);

	double object_distance_to_focus_on;

	if(dist_meter.does_face_an_object())
		object_distance_to_focus_on = dist_meter.get_distance_to_closest_object();
	else
		object_distance_to_focus_on = 9e99; // a very large distance
	
	ApertureCamera apcam = get_ApertureCamera_based_on_free_orbit_camera();
	apcam.set_focus_to(object_distance_to_focus_on);
	std::cout << apcam.get_print();

	const CameraImage* img = acquire_image_with_camera(&apcam);
	img->save(get_snapshot_filename());
}
//------------------------------------------------------------------------------
const CameraImage* FreeOrbitCamera::acquire_image_with_camera(CameraDevice* cam) {
	if(stereo3D) {

		CameraManForStereo3D op(cam);
		op.use_same_stereo_offset_as(Stereo_operator);
		op.aquire_stereo_image(world, settings);
		return op.get_anaglyph_stereo3D_image();
	}else{

		cam->acquire_image(world, settings);
		return cam->get_image();
	}	
}
//------------------------------------------------------------------------------
void FreeOrbitCamera::print_info_of_probing_ray_for_pixel_col_row(int x, int y){

	Ray probing_ray = flying_camera->get_ray_for_pixel_in_row_and_col(y, x);

	const Intersection* intersec = probing_ray.get_first_intersection_in(world);

	UserInteraction::ClearScreen();
	std::stringstream out;

	//      0        1         2         3         4         5         6
	//      123456789012345678901234567890123456789012345678901234567890

	out << " _Info_for_Pixel_(_" << x << "_|_" << y << "_)__________________\n";
	out << "|\n";
	out << "| Ray emitted by camera:\n";
	out << "| " << probing_ray << "\n";
	out << "|\n";
	if( intersec->does_intersect() ){
	out << "| Object: " << intersec->
					get_object()->
					get_path_in_tree_of_frames() << "\n";
	out << "| Distance to first intersection: ";
	out << intersec->get_intersection_distance() << "m\n";
	out << "|\n";
	out << "| In frame of intersecting object\n";
	out << "| | intesection point: ";
	 	out << intersec->get_intersection_vector_in_object_system() << "\n";
	out << "| | surface normal   : ";
	 	out << intersec->get_surface_normal_in_object_system() << "\n";
	out << "| | facing surface   : ";
		if(intersec->from_outside_to_inside())
			out << "outside";
		else
			out << "inside";
	out << "\n";
	out << "|\n";
	out << "| In world frame\n";
	out << "| | intesection point: ";
		out << intersec->get_intersection_vector_in_world_system() << "\n";
	out << "| | surface normal   : ";
	 	out << intersec->get_surface_normal_in_world_system() << "\n";
	
	out << "|\n";		
	out << StringTools::place_first_infront_of_each_new_line_of_second(
		"| ",
		intersec->get_object()->get_print()
	);

	}else{
	out << "| No intersection with any object.\n";	
	}
	out << "|___________________________________________________________\n";
	
	std::cout << out.str();
}
//------------------------------------------------------------------------------
void FreeOrbitCamera::print_free_orbit_help_text()const{

	UserInteraction::ClearScreen();
	std::stringstream out;

	//      0        1         2         3         4         5         6         7         8
	//      12345678901234567890123456789012345678901234567890123456789012345678901234567890
	out << "_Position______________________   _Orientation___________________\n";
	out << " move forward............[ w ]     look up.................[ i ]\n";
	out << " move backward...........[ s ]     look down...............[ k ]\n";
	out << " move left...............[ a ]     look left...............[ j ]\n";
	out << " move right..............[ d ]     look right..............[ l ]\n";
	out << " move up.................[ q ]\n";
	out << " move down...............[ e ]\n";
	out << "\n";
	out << "_Stereo3D_left:red_right:blue__   _Field_of_View_________________\n";
	out << " on/off..................[ t ]     increace................[ n ]\n";
	out << " increase................[ x ]     decreace................[ m ]\n";
	out << " decrease................[ y ]\n";
	out << "                                  _free_orbit___________________\n";
	out << "_Aperture_camera_______________    print help..............[ h ]\n";
	out << " manual focus..[ right mouse ]     print geometry tree.....[ p ]\n";
	out << "                                   exit....................[ESC]\n";
	out << "\n";
	out << "[ left mouse  ] click image for additional information.\n";
	out << "[ right mouse ] click image for manual focus with snapshot.\n";
	out << "\n";
	std::cout << out.str();
}
//------------------------------------------------------------------------------
void FreeOrbitCamera::continue_with_new_scenery_and_settings(
	const Frame *world, 
	const TracerSettings *settings
) {
	this->world = world;
	this->settings = settings;	
	start_free_orbit();
}
//------------------------------------------------------------------------------ 