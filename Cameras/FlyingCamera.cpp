#include "FlyingCamera.h"
using std::cout;
using std::stringstream;
using std::string;
//------------------------------------------------------------------------------
FlyingCamera::FlyingCamera(const Frame *world, const TracerSettings *settings) {	
	this->world = world;
	this->settings = settings;

	flying_camera = new PinHoleCamera("Cam", 
		settings->visual.preview.cols, 
		settings->visual.preview.rows
	);
	
	flying_camera_full_resolution = new PinHoleCamera("Cam", 
		settings->visual.preview.cols*settings->visual.preview.scale, 
		settings->visual.preview.rows*settings->visual.preview.scale
	);

	create_CameraMen_to_safely_operate_the_flying_camera();
	reset_camera();
	time_stamp.update_now();
	
	create_display();
	enter_interactive_display();
}
//------------------------------------------------------------------------------
FlyingCamera::~FlyingCamera() {
	destroy_display();
}
//------------------------------------------------------------------------------
void FlyingCamera::create_CameraMen_to_safely_operate_the_flying_camera() {
	FoV_operator = new CameraManFoV(flying_camera_full_resolution);
	FoV_operator->set_verbosity(true);

	Translation_operator = new CameraManForTranslation(flying_camera_full_resolution);
	Translation_operator->set_verbosity(true);

	Rotation_operator = new CameraManForRotation(flying_camera_full_resolution);
	Rotation_operator->set_verbosity(true);

	Stereo_operator = new CameraManForStereo3D(flying_camera_full_resolution);
	Stereo_operator->set_verbosity(true);
}
//------------------------------------------------------------------------------
void FlyingCamera::reset_camera() {
	Translation_operator->set_default_position(Vector3D(0.0, 0.0, 0.0));
	Rotation_operator->set_default_rotation(Rotation3D(0.0,Deg2Rad(-90.0),0.0));
	FoV_operator->set_default_FoV();
}
//------------------------------------------------------------------------------
void FlyingCamera::enter_interactive_display() {
	UserInteraction::print_welcome_screen();
	print_display_help_text();

	int user_input_key = 0;
	bool key_stroke_requires_image_update = true;

	while(!UserInteraction::is_Escape_key(user_input_key)) {

		user_input_counter++;

		if(it_is_time_again_to_show_the_help())
			print_display_help_text();

		if(key_stroke_requires_image_update)
			update_display();
		
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
			case 'g': Translation_operator->move_to(UserInteraction::get_Vector3D());
			break;
			case UserInteraction::space_key: {
				update_display_full_resolution();
				key_stroke_requires_image_update = false;
			}
			break;
			case 'p': {
				cout << world->get_tree_print() << "\n";
				key_stroke_requires_image_update = false;
			}
			break;
			case 'h': {
				print_display_help_text();
				key_stroke_requires_image_update = false;
			};
			break;
			default: key_stroke_requires_image_update = false;
		}
	}
}
//------------------------------------------------------------------------------
int FlyingCamera::wait_for_user_key_stroke() {
	return cvWaitKey(0);
}
//------------------------------------------------------------------------------
bool FlyingCamera::it_is_time_again_to_show_the_help() {
	if(user_input_counter > 15) {
		user_input_counter = 0;
		return true;
	}else{
		return false;
	}
}
//------------------------------------------------------------------------------
void FlyingCamera::create_display() {
	cv::namedWindow( display_name, CV_WINDOW_AUTOSIZE );
	FlyingCamera* p = this;
	cv::setMouseCallback( 
		display_name.c_str(), 
		mouse_button_event, 
		(void *)p 
	);
}
//------------------------------------------------------------------------------
void FlyingCamera::destroy_display() {
	cv::destroyWindow(display_name);	
}
//------------------------------------------------------------------------------
void FlyingCamera::update_display_full_resolution() {
	cout << "Full resolution image "
	<< flying_camera_full_resolution->get_number_of_sensor_cols() <<"x"
	<< flying_camera_full_resolution->get_number_of_sensor_rows() <<", "
	<< flying_camera_full_resolution->get_number_of_sensor_cols()*
	flying_camera_full_resolution->get_number_of_sensor_rows()/1e6 
	<< " MPixel\n";

	const CameraImage* img = acquire_scaled_image_with_camera(
		false,
		flying_camera_full_resolution
	);

	cv::imshow(display_name, img->Image); 
}
//------------------------------------------------------------------------------
void FlyingCamera::update_display() {
	flying_camera->set_FoV_in_rad(
		flying_camera_full_resolution->get_FoV_in_rad()
	);

	flying_camera->update_position_and_orientation(
		flying_camera_full_resolution->get_position_in_world(),
		flying_camera_full_resolution->get_rotation_in_world()
	);

	const CameraImage* img = acquire_scaled_image_with_camera(true, flying_camera);
	cv::imshow(display_name, img->Image); 
}
//------------------------------------------------------------------------------
void FlyingCamera::mouse_button_event(
	int event, int col, int row, int flags, void *param
) {
	FlyingCamera* p = (FlyingCamera*)param;
	
	if( event == cv::EVENT_LBUTTONDOWN )
		p->print_info_of_probing_ray_for_pixel_col_row( col, row );
	else if( event == cv::EVENT_RBUTTONDOWN )
		p->take_snapshot_manual_focus_on_pixel_col_row( col, row );
	else
		return;
}
//------------------------------------------------------------------------------
void FlyingCamera::toggle_stereo3D() {
	stereo3D = !stereo3D;
	cout << "Stereo 3D : " << (stereo3D ? "On" : "Off") << "\n";
}
//------------------------------------------------------------------------------
string FlyingCamera::get_snapshot_filename() {
	snapshot_counter++;

	stringstream filename;  
	filename << time_stamp.yyyy() << time_stamp.mm() << time_stamp.dd() << "_";
	filename << time_stamp.HH() << time_stamp.MM() << time_stamp.SS();
	filename << "_" << snapshot_counter;
	return filename.str();
}
//------------------------------------------------------------------------------
ApertureCamera FlyingCamera::get_ApertureCamera_based_on_display_camera()const{

	ApertureCamera apcam("Imax70mm", 
		settings->visual.snapshot.cols, 
		settings->visual.snapshot.rows
	);

	apcam.set_fStop_sesnorWidth_rayPerPixel(
		settings->visual.snapshot.focal_length_over_aperture_diameter, 
		settings->visual.snapshot.image_sensor_size_along_a_row, 
		settings->visual.snapshot.rays_per_pixel
	);

	apcam.set_FoV_in_rad(flying_camera_full_resolution->get_FoV_in_rad());
	apcam.update_position_and_orientation(
		flying_camera_full_resolution->get_position_in_world(),
		flying_camera_full_resolution->get_rotation_in_world()
	);
	return apcam;
}
//------------------------------------------------------------------------------
void FlyingCamera::take_snapshot_manual_focus_on_pixel_col_row(int col, int row) {

	Ray probing_ray = 
		flying_camera_full_resolution->get_ray_for_pixel_in_row_and_col(row, col);
	
	DistanceMeter dist_meter(&probing_ray, world);

	double object_distance_to_focus_on;

	if(dist_meter.does_face_an_object())
		object_distance_to_focus_on = dist_meter.get_distance_to_closest_object();
	else
		object_distance_to_focus_on = 9e99; // a very large distance
	
	ApertureCamera apcam = get_ApertureCamera_based_on_display_camera();
	apcam.set_focus_to(object_distance_to_focus_on);
	cout << apcam.get_print();

	const CameraImage* img = acquire_scaled_image_with_camera(false ,&apcam);
	img->save(get_snapshot_filename());
}
//------------------------------------------------------------------------------
const CameraImage* FlyingCamera::acquire_scaled_image_with_camera(
	const bool scale, CameraDevice* cam
) {	
	if(scale){
		
		if(stereo3D) {

			CameraManForStereo3D op(cam);
			op.use_same_stereo_offset_as(Stereo_operator);
			op.aquire_stereo_image(world, settings);
			image = *op.get_anaglyph_stereo3D_image();
			image.scale(settings->visual.preview.scale);
			return &image;
		}else{

			cam->acquire_image(world, settings);
			image = *cam->get_image();
			image.scale(settings->visual.preview.scale);
			return &image;
		}	
	}else{

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
}
//------------------------------------------------------------------------------
void FlyingCamera::print_info_of_probing_ray_for_pixel_col_row(int col, int row) {

	Ray probing_ray = flying_camera_full_resolution->get_ray_for_pixel_in_row_and_col(row, col);

	const Intersection* intersec = probing_ray.get_first_intersection_in(world);

	UserInteraction::ClearScreen();
	stringstream out;

	//      0        1         2         3         4         5         6
	//      123456789012345678901234567890123456789012345678901234567890

	out << " _Info_for_Pixel_(_" << col << "_|_" << row << "_)__________________\n";
	out << "|\n";
	out << "| Ray emitted by camera:\n";
	out << "| " << probing_ray << "\n";
	out << "|\n";
	if(intersec->does_intersect())
		out << get_intersection_info_print(intersec);
	else
		out << "| No intersection with any object.\n";	
	out << "|___________________________________________________________\n";
	cout << out.str();
}
//------------------------------------------------------------------------------
string FlyingCamera::get_intersection_info_print(const Intersection* intersec)const {
	
	stringstream out;
	out << "| Object: " << intersec->get_object()->get_path_in_tree_of_frames() << "\n";
	out << "| Distance to first intersection: " << intersec->get_intersection_distance() << "m\n";
	out << "|\n";
	out << "| In frame of intersecting object\n";
	out << "| | intesection point: " << intersec->get_intersection_vector_in_object_system() << "\n";
	out << "| | surface normal   : " << intersec->get_surface_normal_in_object_system() << "\n";
	out << "| | facing surface   : " << (intersec->from_outside_to_inside() ? "outside" : "inside") << "\n";
	out << "|\n";
	out << "| In world frame\n";
	out << "| | intesection point: " << intersec->get_intersection_vector_in_world_system() << "\n";
	out << "| | surface normal   : " << intersec->get_surface_normal_in_world_system() << "\n";
	out << "|\n";		
	out << StringTools::place_first_infront_of_each_new_line_of_second(
		"| ",
		intersec->get_object()->get_print()
	);
	return out.str();	
}
//------------------------------------------------------------------------------
void FlyingCamera::print_display_help_text()const {

	UserInteraction::ClearScreen();
	stringstream out;

//  0         1         2         3         4         5         6         7         8
//   12345678901234567890123456789012345678901234567890123456789012345678901234567890
	out << 
	"_Position______________________   _Orientation___________________\n"
	" move forward............[ w ]     look up.................[ i ]\n"
	" move backward...........[ s ]     look down...............[ k ]\n"
	" move left...............[ a ]     look left...............[ j ]\n"
	" move right..............[ d ]     look right..............[ l ]\n"
	" move up.................[ q ]\n"
	" move down...............[ e ]\n"
	" move to custom pos......[ g ]\n"
	"\n"
	"_Stereo3D_left:red_right:blue__   _Field_of_View_________________\n"
	" on/off..................[ t ]     increace................[ n ]\n"
	" increase................[ x ]     decreace................[ m ]\n"
	" decrease................[ y ]\n"
	"                                  _interaction__________________\n"
	"_Aperture_camera_______________    print help..............[ h ]\n"
	" manual focus..[ right mouse ]     print geometry tree.....[ p ]\n"
	"                                   exit/next photon........[ESC]\n"
	"\n"
	"[  space key  ] full resolution.\n"
	"[ left mouse  ] click image for additional information.\n"
	"[ right mouse ] click image for manual focus with snapshot.\n\n";
	cout << out.str();
}
//------------------------------------------------------------------------------
void FlyingCamera::continue_with_new_scenery_and_settings(
	const Frame *world, 
	const TracerSettings *settings
) {
	this->world = world;
	this->settings = settings;	
	enter_interactive_display();
}
//------------------------------------------------------------------------------