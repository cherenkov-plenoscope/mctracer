#include "FreeOrbitCamera.h"
//==============================================================================
FreeOrbitCamera::FreeOrbitCamera(
	CartesianFrame *ptr_to_new_world,
	GlobalSettings *ptr_to_new_globalsettings
){
	//free orbit display
	free_orbit_display_name = "Monte Carlo Tracer -> Free Orbit";
	
	// default position for camera
	t_World2Camera.set_null_vector();

	// default orientation for camera
	RotWorld2CameraY_in_rad = -M_PI/2.0;
	RotWorld2CameraZ_in_rad = 0.0;
	
	R_World2Camera.set(
		0.0,
		RotWorld2CameraY_in_rad,
		RotWorld2CameraZ_in_rad
	);

	// default image size and FoV
	FieldOfView_in_rad = M_PI/2.0; //90Deg
	
	Image.Set( MCT_QVGA );
	
	// set up camera
	FlyingPinHoleCamera.set_cam(
		free_orbit_display_name,
		t_World2Camera,
		R_World2Camera,
		Image.Width(),
		Image.Hight()
	);
	
	FlyingPinHoleCamera.set_pin_hole_cam(FieldOfView_in_rad);
	//==================//
	// snapshot
	snapshot_counter = 0;
	
	//==================//
	// stereo view
	stereo_offset_in_m = 0.0;
	stereo_offset_increment_in_m = 0.0025;

	// set the world frame and global settings
	world = ptr_to_new_world;
	settings = ptr_to_new_globalsettings;
}
//==============================================================================
void FreeOrbitCamera::aquire_image(){
	if(stereo_offset_in_m > 0.0){
		FlyingPinHoleCamera.cam_acquire_stereo_anaglyph_image(
			world,
			settings,
			stereo_offset_in_m
		);
	}else{
		FlyingPinHoleCamera.cam_acquire_image_parallel(
			world,
			settings
		);
	}
}
//==============================================================================
void FreeOrbitCamera::update_free_orbit_display(){
	aquire_image();
	cv::imshow(free_orbit_display_name,FlyingPinHoleCamera.get_image()); 
}
//==============================================================================
void FreeOrbitCamera::move_forward(){
	// increment
	double scaling_factor = 0.5/FieldOfView_in_rad;
	
	// calculate current poining direction of camera
	Vector3D CameraPointingDirection=
	FlyingPinHoleCamera.get_pointing_direction();
	
	CameraPointingDirection = 
	CameraPointingDirection/CameraPointingDirection.norm2();

	t_World2Camera = 
	t_World2Camera+CameraPointingDirection*scaling_factor;
	
	FlyingPinHoleCamera.SetPositionAndOrientation(
		t_World2Camera,
		R_World2Camera
	);
	
	FlyingPinHoleCamera.set_pin_hole_cam(FieldOfView_in_rad);	
	
	std::cout<<"free_orbit->move forward: "<<t_World2Camera<<std::endl;
}
//==============================================================================
void FreeOrbitCamera::move_backward(){
	// increment
	double scaling_factor = 0.5/FieldOfView_in_rad;
	
	// calculate current poining direction of camera
	Vector3D CameraPointingDirection=
	FlyingPinHoleCamera.get_pointing_direction();
	
	CameraPointingDirection = 
	CameraPointingDirection/CameraPointingDirection.norm2();
	
	t_World2Camera = 
	t_World2Camera - CameraPointingDirection*scaling_factor;
	
	FlyingPinHoleCamera.SetPositionAndOrientation(
		t_World2Camera,
		R_World2Camera
	);
	
	FlyingPinHoleCamera.set_pin_hole_cam(FieldOfView_in_rad);	
	
	std::cout<<"free_orbit->move backward: "<<t_World2Camera<<std::endl;
}
//==============================================================================
void FreeOrbitCamera::move_left(){
	
	double scaling_factor = 0.5/FieldOfView_in_rad;
	
	Vector3D CameraPointingDirection =
	FlyingPinHoleCamera.get_pointing_direction();
	
	CameraPointingDirection = 
	CameraPointingDirection/CameraPointingDirection.norm2();
	
	Vector3D ez; 
	ez.set_unit_vector_z(); 
	Vector3D vec_left;
	vec_left=ez.CrossProduct(CameraPointingDirection);

	t_World2Camera = t_World2Camera - vec_left*scaling_factor;
	
	FlyingPinHoleCamera.SetPositionAndOrientation(
		t_World2Camera,
		R_World2Camera
	);
	
	FlyingPinHoleCamera.set_pin_hole_cam(FieldOfView_in_rad);
		
	std::cout<<"free_orbit->move left: ";
	std::cout<<t_World2Camera.get_string()<<std::endl;
}
//==============================================================================
void FreeOrbitCamera::move_right(){
	
	double scaling_factor = 0.5/FieldOfView_in_rad;
	
	Vector3D CameraPointingDirection =
	FlyingPinHoleCamera.get_pointing_direction();
	
	CameraPointingDirection = 
	CameraPointingDirection/CameraPointingDirection.norm2();
	
	Vector3D ez; 
	ez.set_unit_vector_z(); 
	Vector3D vec_left;
	vec_left=ez.CrossProduct(CameraPointingDirection);

	t_World2Camera = t_World2Camera + vec_left*scaling_factor;
	
	FlyingPinHoleCamera.SetPositionAndOrientation(
		t_World2Camera,
		R_World2Camera
	);
	
	FlyingPinHoleCamera.set_pin_hole_cam(FieldOfView_in_rad);
		
	std::cout<<"free_orbit->move right: ";
	std::cout<<t_World2Camera.get_string()<<std::endl;
	
}
//==============================================================================
void FreeOrbitCamera::increase_FoV(){
	
	if( FieldOfView_in_rad < Deg2Rad(120.0) ){
		
		FieldOfView_in_rad = FieldOfView_in_rad + Deg2Rad(10.0);
		
		FlyingPinHoleCamera.set_pin_hole_cam(FieldOfView_in_rad);
		
		std::cout<< "free_orbit-> FoV: ";
		std::cout<< Rad2Deg(FieldOfView_in_rad);
		std::cout<< " [DEG]"<<std::endl;
	}
}
//==============================================================================
void FreeOrbitCamera::decrease_FoV(){
	
	if( FieldOfView_in_rad > Deg2Rad(15.0) ){
		
		FieldOfView_in_rad = FieldOfView_in_rad - Deg2Rad(10.0);
		
		FlyingPinHoleCamera.set_pin_hole_cam(FieldOfView_in_rad);
		
		std::cout<< "free_orbit-> FoV: ";
		std::cout<< Rad2Deg(FieldOfView_in_rad);
		std::cout<< " [DEG]"<<std::endl;	
	}
}
//==============================================================================
void FreeOrbitCamera::look_up(){
	double increment_in_rad = FieldOfView_in_rad/25.0;
	
	if(RotWorld2CameraY_in_rad<0.0 + increment_in_rad){
		RotWorld2CameraY_in_rad = RotWorld2CameraY_in_rad + increment_in_rad;
		
		R_World2Camera.set(0.0,RotWorld2CameraY_in_rad,RotWorld2CameraZ_in_rad);

		FlyingPinHoleCamera.SetPositionAndOrientation(
			t_World2Camera,
			R_World2Camera
		);

		FlyingPinHoleCamera.set_pin_hole_cam(FieldOfView_in_rad);					
							
		std::cout<<"free_orbit-> rot y: "<<Rad2Deg(RotWorld2CameraY_in_rad);
		std::cout<<" [DEG]"<<std::endl;
	}
}
//==============================================================================
void FreeOrbitCamera::look_down(){
	double increment_in_rad = FieldOfView_in_rad/25.0;

	if(RotWorld2CameraY_in_rad > -M_PI + increment_in_rad){
		RotWorld2CameraY_in_rad = RotWorld2CameraY_in_rad - increment_in_rad;
		
		R_World2Camera.set(0.0,RotWorld2CameraY_in_rad,RotWorld2CameraZ_in_rad);

		FlyingPinHoleCamera.SetPositionAndOrientation(
			t_World2Camera,
			R_World2Camera
		);

		FlyingPinHoleCamera.set_pin_hole_cam(FieldOfView_in_rad);					
							
		std::cout<<"free_orbit-> rot y: "<<Rad2Deg(RotWorld2CameraY_in_rad);
		std::cout<<" [DEG]"<<std::endl;
	}
}
//==============================================================================
void FreeOrbitCamera::look_left(){
	double increment_in_rad = FieldOfView_in_rad/25.0;
	
	RotWorld2CameraZ_in_rad = fmod(RotWorld2CameraZ_in_rad,(2.0*M_PI));
	
	RotWorld2CameraZ_in_rad = RotWorld2CameraZ_in_rad + increment_in_rad;
	
	R_World2Camera.set(0.0,RotWorld2CameraY_in_rad,RotWorld2CameraZ_in_rad);

	FlyingPinHoleCamera.SetPositionAndOrientation(
		t_World2Camera,
		R_World2Camera
	);

	FlyingPinHoleCamera.set_pin_hole_cam(FieldOfView_in_rad);					
						
	std::cout<<"free_orbit-> rot z: "<<Rad2Deg(RotWorld2CameraZ_in_rad);
	std::cout<<" [DEG]"<<std::endl;
}
//==============================================================================
void FreeOrbitCamera::look_right(){
	double increment_in_rad = FieldOfView_in_rad/25.0;
	
	RotWorld2CameraZ_in_rad = fmod(RotWorld2CameraZ_in_rad,(2.0*M_PI));
	
	RotWorld2CameraZ_in_rad = RotWorld2CameraZ_in_rad - increment_in_rad;
	
	R_World2Camera.set(0.0,RotWorld2CameraY_in_rad,RotWorld2CameraZ_in_rad);

	FlyingPinHoleCamera.SetPositionAndOrientation(
		t_World2Camera,
		R_World2Camera
	);

	FlyingPinHoleCamera.set_pin_hole_cam(FieldOfView_in_rad);					
						
	std::cout<<"free_orbit-> rot z: "<<Rad2Deg(RotWorld2CameraZ_in_rad);
	std::cout<<" [DEG]"<<std::endl;
}
//==============================================================================
void FreeOrbitCamera::take_snapshot(){
	snapshot_counter++;
	
	ApertureCamera Mamiya645;
	// The real Mamiya Sekor has F=2.3 here it is "dreamlens" setup with F=0.95
	double Mamiya_F_stop_number = 0.95;
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
		Mamiya_number_of_rays_emitted_per_pixel
	);
	
	Vector3D center_ray_direction;
	center_ray_direction.set_unit_vector_z();
	HomoTrafo3D rot_mat; 
	rot_mat.set_transformation(R_World2Camera,t_World2Camera);
	rot_mat.transform_orientation(&center_ray_direction);
	
	Ray center_ray_of_camera;
	center_ray_of_camera.SetRay(t_World2Camera,center_ray_direction);
	double object_distance = 
	center_ray_of_camera.get_distance_to_closest_object(
		world,
		0,
		NULL,
		settings,
		0.0
	);
	
	if(object_distance==0.0) // default object distance is set to 25m
		object_distance = 25.0;
		
	Mamiya645.set_object_distance(object_distance);
	Mamiya645.disp();
	
	std::stringstream export_name_Mamiya645;  
	export_name_Mamiya645<<"snap_"<<snapshot_counter<<"_Mamiya645";
	
	if(stereo_offset_in_m > 0.0){
		Mamiya645.cam_acquire_stereo_anaglyph_image
		(world,settings,stereo_offset_in_m);
	}else{
		Mamiya645.cam_acquire_image
		(world,settings);
	}
								
	Mamiya645.save_image(export_name_Mamiya645.str());
								
	std::cout<<"free_orbit-> snapshot:"<<std::endl;
}
//==============================================================================
void FreeOrbitCamera::display_help()const{

	ClearScreen();
	std::stringstream out;

	//    0        1         2         3         4         5         6
	//    123456789012345678901234567890123456789012345678901234567890

	out<<"______________________________________________________\n";
	out<<"| HELP for >free orbit<\n";
	out<<"|\n";
	out<<"| __camera_position____________\n";
	out<<"| | move forward............[w]\n";
	out<<"| | move backward...........[s]\n";
	out<<"| | move left...............[a]\n";
	out<<"| | move right..............[d]\n";
	out<<"| |____________________________\n";
	out<<"|\n";
	out<<"| __camera_orientation_________\n";
	out<<"| | look up.................[i]\n";
	out<<"| | look down...............[k]\n";
	out<<"| | look left...............[j]\n";
	out<<"| | look right..............[l]\n";
	out<<"| |____________________________\n";
	out<<"|\n";
	out<<"| __stereo_3D_anaglyph_red/blue\n";
	out<<"| | increase offset.........[x]\n";
	out<<"| | decrease offset.........[y]\n";
	out<<"| |____________________________\n";
	out<<"|\n";
	out<<"| __camera_field_of_view_______\n";
	out<<"| | increace FoV............[+]\n";
	out<<"| | decreace FoV............[-]\n";
	out<<"| |____________________________\n";
	out<<"|\n";
	out<<"| __Snapshot_Mamiya645_f80mm_F2.8\n";
	out<<"| | take snapshot...........[n]\n";
	out<<"| |____________________________\n";
	out<<"|\n";
	out<<"| __exit_free_orbit____________\n";
	out<<"| | exit..................[ESC]\n";
	out<<"| |____________________________\n";
	out<<"|_____________________________________________________\n";
	
	std::cout<<out.str();
}
//==============================================================================
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
//==============================================================================
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
//==============================================================================
void FreeOrbitCamera::onMouse(int event, int x, int y, int flags, void *param){
		
		FreeOrbitCamera* p = (FreeOrbitCamera*)param;
		
		if( event != cv::EVENT_LBUTTONDOWN )
		return;
		// todo
		p->DispImageInfo( x, y );
		//std::cout << "You pressed the left mouse button on ("<<x<<"|"<<y<<")" << std::endl;
}
//==============================================================================
void FreeOrbitCamera::DispImageInfo(int x, int y){

	// get the corresponding ray 
	Ray Line = FlyingPinHoleCamera.cam_send_ray( y, x );

	Intersection* ClosestIntersection = Line.get_closest_intersection(
		world,
		settings
	);

	ClearScreen();
	std::stringstream out;

	//      0        1         2         3         4         5         6
	//      123456789012345678901234567890123456789012345678901234567890

	out << " _Info_for_Pixel_(_" << x << "_|_" << y << "_)__________________\n";
	out << "|\n";
	out << "| Ray emitted by camera:\n";
	out << "| " << Line << "\n";
	out << "|\n";
	if( ClosestIntersection->get_intersection_flag() ){

	out << "| Distance to first intersection: ";
	out << ClosestIntersection->get_intersection_distance() << " [m]\n";
	out << "|\n";
	//out << "| ID of Object: ";
	//out << 	ClosestIntersection->get_pointer_to_intersecting_object()->
	//		get_ID() << "\n";
	out << "| Name of Object: " << *ClosestIntersection->
					get_pointer_to_intersecting_object()->
					get_pointer_to_name_of_frame() << "\n";

	out << "| Path of Object: " << ClosestIntersection->
					get_pointer_to_intersecting_object()->
					get_path() << "\n";
					
	out << "|\n";
	out << "| In frame of intersecting object___________________________\n";
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
	
	std::cout<<out.str();
}
//==============================================================================
void FreeOrbitCamera::start_free_orbit(){
	display_help();
	int key = 0;
	bool key_stroke_requires_image_update = true;
	
	cv::namedWindow( free_orbit_display_name, CV_WINDOW_AUTOSIZE );

	FreeOrbitCamera* p = this;
	cv::setMouseCallback( free_orbit_display_name.c_str(), onMouse, (void *)p );

	while( key != 27 ){

		if( key_stroke_requires_image_update ){
			update_free_orbit_display();
		}
		
		key_stroke_requires_image_update = true;
		key = cvWaitKey(0);

		switch(key){
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
				key_stroke_requires_image_update = false;
			};
			break;
			case 'h' : {
				display_help();
				key_stroke_requires_image_update = false;
			};
			break;
			default: key_stroke_requires_image_update = false;
		}
	}
	cv::destroyWindow(free_orbit_display_name);
}
