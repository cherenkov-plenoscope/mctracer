#include "FreeOrbitCamera.h"
//==============================================================================
FreeOrbitCamera::FreeOrbitCamera(
	CartesianFrame *world,
	GlobalSettings *settings
){	
	this->world = world;
	this->settings = settings;

	Image.Set( MCT_VGA );

	flying_camera = new PinHoleCamera("Cam",Image.Width(), Image.Hight());

	create_CameraMen_to_safely_operate_the_flying_camera();

	reset_camera();
	start_free_orbit();
}
//------------------------------------------------------------------------------
void FreeOrbitCamera::create_CameraMen_to_safely_operate_the_flying_camera(){
	FoV_operator = new CameraManFoV(flying_camera);
	FoV_operator->set_verbosity(true);

	Translation_operator = new CameraManForTranslation(flying_camera);

	Rotation_operator = new CameraManForRotation(flying_camera);
	Rotation_operator->set_verbosity(true);

	Stereo_operator = new CameraManForStereo3D(flying_camera);
}
//==============================================================================
void FreeOrbitCamera::reset_camera(){
	Translation_operator->set_default_position();
	Rotation_operator->set_default_rotation();
	FoV_operator->set_default_FoV();
}
//==============================================================================
void FreeOrbitCamera::start_free_orbit(){

	print_free_orbit_help_text();
	create_free_orbit_display();

	int user_input_key = 0;
	bool key_stroke_requires_image_update = true;

	while( is_not_Escape(user_input_key) ){

		if( key_stroke_requires_image_update ){
			update_free_orbit_display();
		}
		
		key_stroke_requires_image_update = true;
		user_input_key = cvWaitKey(0);

		switch(user_input_key){
			case 't': toggle_stereo3D();
			break;
			case 'w': Translation_operator->move_forward();
			break;
			case 's': Translation_operator->move_backward();
			break;
			case 'a': Translation_operator->move_left();
			break;
			case 'd': Translation_operator->move_right();
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
			case 'g': {
				take_snapshot();
				key_stroke_requires_image_update = false;
			};
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
//==============================================================================
void FreeOrbitCamera::create_free_orbit_display(){
	cv::namedWindow( free_orbit_display_name, CV_WINDOW_AUTOSIZE );

	FreeOrbitCamera* p = this;

	cv::setMouseCallback( 
		free_orbit_display_name.c_str(), 
		left_mouse_button_event, 
		(void *)p 
	);
}
//==============================================================================
void FreeOrbitCamera::destroy_free_orbit_display(){
	cv::destroyWindow(free_orbit_display_name);	
}
//==============================================================================
void FreeOrbitCamera::update_free_orbit_display(){
	if(stereo3D){
		Stereo_operator->aquire_stereo_image(world, settings);
		cv::imshow(
			free_orbit_display_name,
			*(Stereo_operator->get_anaglyph_stereo3D_image()->Image)
		); 
	}else{
		flying_camera->acquire_image(world, settings);
		cv::imshow(
			free_orbit_display_name,
			*(flying_camera->get_image()->Image)
		); 
	}
}
//==============================================================================
void FreeOrbitCamera::left_mouse_button_event(
	int event, int x, int y, int flags, void *param
){
		FreeOrbitCamera* p = (FreeOrbitCamera*)param;
		
		if( event != cv::EVENT_LBUTTONDOWN )
			return;

		p->print_info_of_probing_ray_for_pixel_x_y( x, y );
}
//==============================================================================
void FreeOrbitCamera::toggle_stereo3D(){
	stereo3D = !stereo3D;
	std::cout << get_prefix_print() << "Stereo 3D : ";
	std::cout << ((stereo3D == true)? "On": "Off") << "\n";
}
//==============================================================================
bool FreeOrbitCamera::is_not_Escape(const int user_input_key)const{
	return user_input_key != 27;
}
//==============================================================================
void FreeOrbitCamera::take_snapshot(){
	
	ApertureCamera Mamiya645 = get_Mamiya645_based_on_free_orbit_camera();

	Mamiya645.auto_focus(world, settings);

	Mamiya645.print();

	if(stereo3D){
		std::cout << get_prefix_print() << "stereo snapshot:\n";

		CameraManForStereo3D op(&Mamiya645);
		op.use_same_stereo_offset_as(Stereo_operator);
		op.aquire_stereo_image(world, settings);
		op.get_anaglyph_stereo3D_image()->
			save_image_to_file(get_snapshot_filename());
	}else{
		std::cout << get_prefix_print() << "snapshot:\n";

		Mamiya645.acquire_image(world, settings);
		Mamiya645.save_image(get_snapshot_filename());
	}
}
//==============================================================================
std::string FreeOrbitCamera::get_snapshot_filename(){
	snapshot_counter++;

	std::stringstream filename;  
	filename << "snap_" << snapshot_counter << "_Mamiya645";
	return filename.str();
}
//==============================================================================
ApertureCamera FreeOrbitCamera::get_Mamiya645_based_on_free_orbit_camera()const{

	ApertureCamera Mamiya645("Mamiya645", 4*Image.Width(), 4*Image.Hight());

	// The real Mamiya Sekor has F=2.3 here it is "dreamlens" setup with F=0.95
	double Mamiya_F_stop_number = 0.95;
	double Mamiya_sensor_width_in_m = 0.06;
	uint   Mamiya_number_of_rays_emitted_per_pixel = 5;
	
	Mamiya645.set_aperture_cam(
		Mamiya_F_stop_number,
		Mamiya_sensor_width_in_m,
		Mamiya_number_of_rays_emitted_per_pixel
	);

	Mamiya645.set_FoV_in_rad(flying_camera->get_FoV_in_rad());

	Mamiya645.update_position_and_orientation(
		flying_camera->get_position_in_world(),
		flying_camera->get_rotation_in_world()
	);

	return Mamiya645;
}
//==============================================================================
std::string FreeOrbitCamera::get_prefix_print()const{
	return "free_orbit -> ";
}
//==============================================================================
void FreeOrbitCamera::print_info_of_probing_ray_for_pixel_x_y(int x, int y){

	Ray probing_ray = flying_camera->get_ray_for_pixel_in_row_and_col(y, x);

	Intersection* ClosestIntersection = probing_ray.get_closest_intersection(
		world,
		settings
	);

	UserInteraction::ClearScreen();
	std::stringstream out;

	//      0        1         2         3         4         5         6
	//      123456789012345678901234567890123456789012345678901234567890

	out << " _Info_for_Pixel_(_" << x << "_|_" << y << "_)__________________\n";
	out << "|\n";
	out << "| Ray emitted by camera:\n";
	out << "| " << probing_ray << "\n";
	out << "|\n";
	if( ClosestIntersection->get_intersection_flag() ){

	out << "| Distance to first intersection: ";
	out << ClosestIntersection->get_intersection_distance() << " [m]\n";
	out << "|\n";
	out << "| Name of Object: " << ClosestIntersection->
					get_pointer_to_intersecting_object()->
					get_name_of_frame() << "\n";

	out << "| Path of Object: " << ClosestIntersection->
					get_pointer_to_intersecting_object()->
					get_path() << "\n";
					
	out << "|\n";
	out << "|  _In frame of intersecting object_________________________\n";
	out << "| |\n";
	out << "| | intesection point: ";
	 	out<< ClosestIntersection->
	 	get_intersection_point_in_object_system() << "\n";
	out << "| | surface normal   : ";
	 	out<< ClosestIntersection->
	 	get_surface_normal_in_object_system() << "\n";
	out << "| |_________________________________________________________\n";
	}else{
	out << "| No Intersection with an object.\n";	
	}
	out << "|___________________________________________________________\n";
	
	std::cout << out.str();
}
//==============================================================================
void FreeOrbitCamera::print_free_orbit_help_text()const{

	UserInteraction::ClearScreen();
	std::stringstream out;

	//      0        1         2         3         4         5         6         7         8
	//      12345678901234567890123456789012345678901234567890123456789012345678901234567890
	out << "_Camera_position_______________   _Camera_orientation____________\n";
	out << " move forward............[ w ]     look up.................[ i ]\n";
	out << " move backward...........[ s ]     look down...............[ k ]\n";
	out << " move left...............[ a ]     look left...............[ j ]\n";
	out << " move right..............[ d ]     look right..............[ l ]\n";
	out << "\n";
	out << "_Stereo3D_left:red_right:blue__   _camera_Field_of_View__________\n";
	out << " toggle stereo 3D........[ t ]     increace FoV............[ n ]\n";
	out << " increase offset.........[ x ]     decreace FoV............[ m ]\n";
	out << " decrease offset.........[ y ]\n";
	out << "                                  _free_orbit___________________\n";
	out << "_Mamiya645_medium_format_camera    help....................[ h ]\n";
	out << " take snapshot...........[ g ]     exit....................[ESC]\n";
	out << "\n";
	std::cout << out.str();
}