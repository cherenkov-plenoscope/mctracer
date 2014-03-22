#include "FreeOrbitCamera.h"
//======================================================================
FreeOrbitCamera::FreeOrbitCamera(){
	//free orbit display
	free_orbit_display_name = "free orbit";
	
	flag_world_and_settings_are_set = false;
	
	// default position and orientation for pin_hole_camera
	t_World2Camera.set_null_vector();
	RotWorld2CameraY_in_rad = -M_PI/2.0;
	RotWorld2CameraZ_in_rad = 0.0;
	
	R_World2Camera.set(
	0.0,
	RotWorld2CameraY_in_rad,
	RotWorld2CameraZ_in_rad
	);

	// default image size and FoV
	//stereo_view  = false;
	ImageResolutionInX = 450;
	ImageResolutionInY = 338;
	FieldOfView_in_rad = M_PI/2.0; //90Deg
	
	// set up camera
	FlyingPinHoleCamera.set_cam(
	free_orbit_display_name,
	t_World2Camera,
	R_World2Camera,
	ImageResolutionInX,
	ImageResolutionInY
	);
	
	FlyingPinHoleCamera.set_pin_hole_cam(FieldOfView_in_rad);
	//==================//
	// snapshot
	snapshot_counter = 0;
	
	//==================//
	// stereo view
	stereo_offset_in_m = 0.0;
	stereo_offset_increment_in_m = 0.0025;
}
//======================================================================
void FreeOrbitCamera::set_free_orbit(CartesianFrame *ptr_to_new_world,
					GlobalSettings *ptr_to_new_globalsettings){
	ptr_to_world = ptr_to_new_world;
	ptr_to_globalsettings = ptr_to_new_globalsettings;
	flag_world_and_settings_are_set = true;
}
//======================================================================
void FreeOrbitCamera::aquire_image(){
	if(flag_world_and_settings_are_set){
		
		if(stereo_offset_in_m > 0.0){
			FlyingPinHoleCamera.
			cam_acquire_stereo_anaglyph_image(
			ptr_to_world,
			ptr_to_globalsettings,
			stereo_offset_in_m
			);
		}else{
			FlyingPinHoleCamera.
			cam_acquire_image_parallel(
			ptr_to_world,
			ptr_to_globalsettings
			);
		}

	}else{
		std::cout<<"free_orbit->aquire_image():";
		std::cout<<"Can not aquire image because";
		std::cout<<" world and global settings are not set!"<<std::endl;
	}
}
//======================================================================
void FreeOrbitCamera::update_free_orbit_display(){
	aquire_image();
	cv::imshow(free_orbit_display_name,FlyingPinHoleCamera.get_image()); 
}
//======================================================================
void FreeOrbitCamera::move_forward(){
	// increment
	double dbl_increment = 0.5/FieldOfView_in_rad;
	
	// calculate current poining direction of camera
	Vector3D CameraPointingDirection=
	FlyingPinHoleCamera.get_pointing_direction();
	
	CameraPointingDirection = 
	CameraPointingDirection/CameraPointingDirection.norm2();

	t_World2Camera = 
	t_World2Camera+CameraPointingDirection*dbl_increment;
	
	FlyingPinHoleCamera.set_cam(
	free_orbit_display_name,
	t_World2Camera,
	R_World2Camera,
	ImageResolutionInX,
	ImageResolutionInY);
	
	FlyingPinHoleCamera.set_pin_hole_cam(FieldOfView_in_rad);	
	
	std::cout<<"free_orbit->move forward: "<<t_World2Camera<<std::endl;
}
//======================================================================
void FreeOrbitCamera::move_backward(){
	// increment
	double dbl_increment = 0.5/FieldOfView_in_rad;
	
	// calculate current poining direction of camera
	Vector3D CameraPointingDirection=
	FlyingPinHoleCamera.get_pointing_direction();
	
	CameraPointingDirection = 
	CameraPointingDirection/CameraPointingDirection.norm2();
	
	t_World2Camera = 
	t_World2Camera - CameraPointingDirection*dbl_increment;
	
	FlyingPinHoleCamera.set_cam(
	free_orbit_display_name,
	t_World2Camera,
	R_World2Camera,
	ImageResolutionInX,
	ImageResolutionInY);
	
	FlyingPinHoleCamera.set_pin_hole_cam(FieldOfView_in_rad);	
	
	std::cout<<"free_orbit->move backward: "<<t_World2Camera<<std::endl;
}
//======================================================================
void FreeOrbitCamera::move_left(){
	
	double dbl_increment = 0.5/FieldOfView_in_rad;
	
	Vector3D CameraPointingDirection =
	FlyingPinHoleCamera.get_pointing_direction();
	
	CameraPointingDirection = 
	CameraPointingDirection/CameraPointingDirection.norm2();
	
	Vector3D ez; 
	ez.set_unit_vector_z(); 
	Vector3D vec_left;
	vec_left=ez.cross_product(CameraPointingDirection);

	t_World2Camera = t_World2Camera - vec_left*dbl_increment;
	
	FlyingPinHoleCamera.set_cam(
	free_orbit_display_name,
	t_World2Camera,
	R_World2Camera,
	ImageResolutionInX,
	ImageResolutionInY);
	
	FlyingPinHoleCamera.set_pin_hole_cam(FieldOfView_in_rad);
		
	std::cout<<"free_orbit->move left: ";
	std::cout<<t_World2Camera.get_string()<<std::endl;
}
//======================================================================
void FreeOrbitCamera::move_right(){
	
	double dbl_increment = 0.5/FieldOfView_in_rad;
	
	Vector3D CameraPointingDirection =
	FlyingPinHoleCamera.get_pointing_direction();
	
	CameraPointingDirection = 
	CameraPointingDirection/CameraPointingDirection.norm2();
	
	Vector3D ez; 
	ez.set_unit_vector_z(); 
	Vector3D vec_left;
	vec_left=ez.cross_product(CameraPointingDirection);

	t_World2Camera = t_World2Camera + vec_left*dbl_increment;
	
	FlyingPinHoleCamera.set_cam(
	free_orbit_display_name,
	t_World2Camera,
	R_World2Camera,
	ImageResolutionInX,
	ImageResolutionInY);
	
	FlyingPinHoleCamera.set_pin_hole_cam(FieldOfView_in_rad);
		
	std::cout<<"free_orbit->move right: ";
	std::cout<<t_World2Camera.get_string()<<std::endl;
	
}
//======================================================================
void FreeOrbitCamera::increase_FoV(){
	
	if( FieldOfView_in_rad < ((M_PI*2.0)/360)*120.0 ){
		
		FieldOfView_in_rad = FieldOfView_in_rad + ((M_PI*2.0)/360)*10.0;
		
		FlyingPinHoleCamera.set_pin_hole_cam(FieldOfView_in_rad);
		
		std::cout<<"free_orbit-> FoV: ";
		std::cout<<FieldOfView_in_rad*360.0/(2.0*M_PI);
		std::cout<<" [DEG]"<<std::endl;
		
	}
	
}
//======================================================================
void FreeOrbitCamera::decrease_FoV(){
	
	if( FieldOfView_in_rad > ((M_PI*2.0)/360)*15.0 ){
		
		FieldOfView_in_rad = FieldOfView_in_rad - ((M_PI*2.0)/360)*10.0;
		
		FlyingPinHoleCamera.set_pin_hole_cam(FieldOfView_in_rad);
		
		std::cout<<"free_orbit-> FoV: ";
		std::cout<<FieldOfView_in_rad*360.0/(2.0*M_PI);
		std::cout<<" [DEG]"<<std::endl;
		
	}
	
}
//======================================================================
void FreeOrbitCamera::look_up(){
	double dbl_step = FieldOfView_in_rad/25.0;
	
	if(RotWorld2CameraY_in_rad<0.0 + dbl_step){
	RotWorld2CameraY_in_rad = RotWorld2CameraY_in_rad + dbl_step;
	
	R_World2Camera.set(0.0,RotWorld2CameraY_in_rad,RotWorld2CameraZ_in_rad);
	FlyingPinHoleCamera.set_cam(free_orbit_display_name,
						t_World2Camera,R_World2Camera,ImageResolutionInX,ImageResolutionInY);
	FlyingPinHoleCamera.set_pin_hole_cam(FieldOfView_in_rad);					
						
	std::cout<<"free_orbit-> rot y: "<<RotWorld2CameraY_in_rad*360.0/(2.0*M_PI);
	std::cout<<" [DEG]"<<std::endl;
	}
}
//======================================================================
void FreeOrbitCamera::look_down(){
	double dbl_step = FieldOfView_in_rad/25.0;

	if(RotWorld2CameraY_in_rad > -M_PI + dbl_step){
	RotWorld2CameraY_in_rad = RotWorld2CameraY_in_rad - dbl_step;
	
	R_World2Camera.set(0.0,RotWorld2CameraY_in_rad,RotWorld2CameraZ_in_rad);
	FlyingPinHoleCamera.set_cam(free_orbit_display_name,
						t_World2Camera,R_World2Camera,ImageResolutionInX,ImageResolutionInY);
	FlyingPinHoleCamera.set_pin_hole_cam(FieldOfView_in_rad);					
						
	std::cout<<"free_orbit-> rot y: "<<RotWorld2CameraY_in_rad*360.0/(2.0*M_PI);
	std::cout<<" [DEG]"<<std::endl;
	}
}
//======================================================================
void FreeOrbitCamera::look_left(){
	double dbl_step = FieldOfView_in_rad/25.0;
	
	RotWorld2CameraZ_in_rad = fmod(RotWorld2CameraZ_in_rad,(2.0*M_PI));
	
	RotWorld2CameraZ_in_rad = RotWorld2CameraZ_in_rad + dbl_step;
	
	R_World2Camera.set(0.0,RotWorld2CameraY_in_rad,RotWorld2CameraZ_in_rad);
	FlyingPinHoleCamera.set_cam(free_orbit_display_name,
						t_World2Camera,R_World2Camera,ImageResolutionInX,ImageResolutionInY);
	FlyingPinHoleCamera.set_pin_hole_cam(FieldOfView_in_rad);					
						
	std::cout<<"free_orbit-> rot z: "<<RotWorld2CameraZ_in_rad*360.0/(2.0*M_PI);
	std::cout<<" [DEG]"<<std::endl;
}
//======================================================================
void FreeOrbitCamera::look_right(){
	double dbl_step = FieldOfView_in_rad/25.0;
	
	RotWorld2CameraZ_in_rad = fmod(RotWorld2CameraZ_in_rad,(2.0*M_PI));
	
	RotWorld2CameraZ_in_rad = RotWorld2CameraZ_in_rad - dbl_step;
	
	R_World2Camera.set(0.0,RotWorld2CameraY_in_rad,RotWorld2CameraZ_in_rad);
	FlyingPinHoleCamera.set_cam(free_orbit_display_name,
						t_World2Camera,R_World2Camera,ImageResolutionInX,ImageResolutionInY);
	FlyingPinHoleCamera.set_pin_hole_cam(FieldOfView_in_rad);					
						
	std::cout<<"free_orbit-> rot z: "<<RotWorld2CameraZ_in_rad*360.0/(2.0*M_PI);
	std::cout<<" [DEG]"<<std::endl;
}
//======================================================================
void FreeOrbitCamera::take_snapshot(){
	snapshot_counter++;
	
	ApertureCamera Mamiya645;
	double Mamiya_F_stop_number = 2.3;
	double Mamiya_focal_length_in_m = 0.08;
	double Mamiya_sensor_width_in_m = 0.06;
	double Mamiya_sensor_hight_in_m = 0.045;
	double Mamiya_number_of_rays_emitted_per_pixel = 5;
	
	Mamiya645.set_cam("Mamiya645",t_World2Camera,R_World2Camera,1280,1);
	Mamiya645.set_aperture_cam(
		Mamiya_F_stop_number,
		Mamiya_focal_length_in_m,
		Mamiya_sensor_width_in_m,
		Mamiya_sensor_hight_in_m,
		Mamiya_number_of_rays_emitted_per_pixel);
	
	Vector3D center_ray_direction;
	center_ray_direction.set_unit_vector_z();
	HomoTrafo3D rot_mat; rot_mat.set_transformation(R_World2Camera,t_World2Camera);
	rot_mat.transform_orientation(&center_ray_direction);
	
	Ray center_ray_of_camera;
	center_ray_of_camera.set_ray(t_World2Camera,center_ray_direction);
	double object_distance = 
	center_ray_of_camera.get_distance_to_closest_object(
	ptr_to_world,
	0,
	NULL,
	ptr_to_globalsettings,
	0.0
	);
	
	if(object_distance==0.0){
		object_distance = 25.0;
	}
		
	Mamiya645.set_object_distance(object_distance);
	Mamiya645.disp();
	
	std::stringstream export_name_Mamiya645;  
	export_name_Mamiya645<<"snap_"<<snapshot_counter<<"_Mamiya645";
	
	if(stereo_offset_in_m > 0.0){
		Mamiya645.cam_acquire_stereo_anaglyph_image
		(ptr_to_world,ptr_to_globalsettings,stereo_offset_in_m);
	}else{
		Mamiya645.cam_acquire_image
		(ptr_to_world,ptr_to_globalsettings);
	}
								
	Mamiya645.save_image(export_name_Mamiya645.str());
								
	std::cout<<"free_orbit-> snapshot:"<<std::endl;
}
//======================================================================
void FreeOrbitCamera::display_help()const{
	std::stringstream out;
	out<<"______________________________________________________"<<std::endl;
	out<<"| HELP for >free orbit<"<<std::endl;
	out<<"|"<<std::endl;
	out<<"| __camera_position____________"<<std::endl;
	out<<"| | move forward............[w]"<<std::endl;
	out<<"| | move backward...........[s]"<<std::endl;
	out<<"| | move left...............[a]"<<std::endl;
	out<<"| | move right..............[d]"<<std::endl;
	out<<"| |____________________________"<<std::endl;
	out<<"|"<<std::endl;
	out<<"| __camera_orientation_________"<<std::endl;
	out<<"| | look up.................[i]"<<std::endl;
	out<<"| | look down...............[k]"<<std::endl;
	out<<"| | look left...............[j]"<<std::endl;
	out<<"| | look right..............[l]"<<std::endl;
	out<<"| |____________________________"<<std::endl;
	out<<"|"<<std::endl;
	out<<"| __stereo_3D_anaglyph_red/blue"<<std::endl;
	out<<"| | increase offset.........[x]"<<std::endl;
	out<<"| | decrease offset.........[y]"<<std::endl;
	out<<"| |____________________________"<<std::endl;
	out<<"|"<<std::endl;
	out<<"| __camera_field_of_view_______"<<std::endl;
	out<<"| | increace FoV............[+]"<<std::endl;
	out<<"| | decreace FoV............[-]"<<std::endl;
	out<<"| |____________________________"<<std::endl;
	out<<"|"<<std::endl;
	out<<"| __Snapshot_Mamiya645_f80mm_F2.8"<<std::endl;
	out<<"| | take snapshot...........[n]"<<std::endl;
	out<<"| |____________________________"<<std::endl;
	out<<"|"<<std::endl;
	out<<"| __exit_free_orbit____________"<<std::endl;
	out<<"| | exit..................[ESC]"<<std::endl;
	out<<"| |____________________________"<<std::endl;
	out<<"|_____________________________________________________"<<std::endl;
	
	int system_call_return_value = system("clear");
	std::cout<<out.str();
}
//======================================================================
void FreeOrbitCamera::increase_stereo_offset(){
	
	if(stereo_offset_in_m + stereo_offset_increment_in_m < 0.1){
		stereo_offset_in_m += stereo_offset_increment_in_m;
		std::cout<<"free_orbit-> stereo offset increased to: ";
		std::cout<<stereo_offset_in_m<<"[m]"<<std::endl;
	}else{
		std::cout<<"free_orbit-> stereo offset is maximal: ";
		std::cout<<stereo_offset_in_m<<"[m]"<<std::endl;
	}
}
//======================================================================
void FreeOrbitCamera::decrease_stereo_offset(){
			
	if(stereo_offset_in_m - stereo_offset_increment_in_m > 
	stereo_offset_increment_in_m/2.0){
		stereo_offset_in_m -= stereo_offset_increment_in_m;
		std::cout<<"free_orbit-> stereo offset decreased to: ";
		std::cout<<stereo_offset_in_m<<"[m]"<<std::endl;
	}else{	
		stereo_offset_in_m = 0.0;
		std::cout<<"free_orbit-> stereo offset is minimal: ";
		std::cout<<stereo_offset_in_m<<"[m]"<<std::endl;
	}
}
//======================================================================
void FreeOrbitCamera::start_free_orbit(){
	display_help();
	int int_key=0;
	bool flag_int_key_stroke_requires_image_update = true;
	
	cv::namedWindow(free_orbit_display_name, CV_WINDOW_AUTOSIZE );
	while(int_key!=27)
	{
		if(flag_int_key_stroke_requires_image_update){
		update_free_orbit_display();
		}
		
		flag_int_key_stroke_requires_image_update=true;
		int_key=cvWaitKey(0);

		switch(int_key){
			case 'w': move_forward();
			break;
			case 's': move_backward();
			break;
			case 'a': move_left();
			break;
			case 'd': move_right();
			break;
			case '+': increase_FoV();
			break;
			case '-': decrease_FoV();
			break;
			case 'i' : look_up();
			break;
			case 'k' : look_down();
			break;
			case 'j' : look_left();
			break;
			case 'l' : look_right();
			break;
			case 'x' : increase_stereo_offset();
			break;
			case 'y' : decrease_stereo_offset();
			break;
			case 'n' : {
				take_snapshot();
				flag_int_key_stroke_requires_image_update = false;
			};
			break;
			case 'h' : {
				display_help();
				flag_int_key_stroke_requires_image_update = false;
			};
			break;
			default: flag_int_key_stroke_requires_image_update = false;
		}
	}
	cv::destroyWindow(free_orbit_display_name);
}
