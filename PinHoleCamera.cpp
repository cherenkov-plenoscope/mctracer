#include "PinHoleCamera.h"
//======================
void PinHoleCamera::set_pin_hole_cam(double fov){
	//set field of view
	FieldOfView_in_Rad=fov;
	/*
	 calculate principal point (intersection of optical axis and
	 image sensor plane)
	 calculate distance of camera base and principal point
	 using FieldOfView_in_Rad angle 
	
		         camera base
		              /|  \
		             / |  |
		            /  |  |
		           /   |  |
		          /    |  |
		         /fov/2|  |
		        /______|  |
		       /       |   \
		      /        |   /dist_camera_base_2_principal_point
		     /         |  |
		    /          |  |
		   /       ____|  |
		  /       | o  |  |
		 /________|____|  /
		        principal_point
		\______  ______/
		       \/
		      Uu/2
	*/
	
	//distance
	dist_camera_base_2_principal_point =
	((SensorResolutionU/2)/tan(FieldOfView_in_Rad/2));
	
	// principal point
	PrincipalPointOfSensorPlane = 
	CameraPointingDirection*dist_camera_base_2_principal_point;
	
	// calculate sensor vectors
	SensorDirectionU.set_unit_vector_x();
	SensorDirectionV.set_unit_vector_y();
	T_Camera2World.transform_orientation(&SensorDirectionU);
	T_Camera2World.transform_orientation(&SensorDirectionV);
	
	// allocate memory for image
	allocate_memory_for_image(SensorResolutionV,SensorResolutionU);
}
//======================
std::string PinHoleCamera::get_pin_hole_cam_string(){
	std::stringstream out; out.str("");
	out<<get_cam_string();
	out<<"||field of view: "<<FieldOfView_in_Rad<<" [rad] ";
	out<<"/ "<<(FieldOfView_in_Rad*360.0)/(2.0*M_PI)<<" [deg]"<<std::endl;
	out<<"||resolution: u x v : ";
	out<<SensorResolutionU<<"x";
	out<<SensorResolutionV<<" [pixels]";
	out<<" / "<<SensorResolutionUTimesV/1e6<<"[M pixels]"<<std::endl;
	out<<"||distance base to principalpoint: ";
	out<<dist_camera_base_2_principal_point<<" [m]"<<std::endl;
	return out.str();
}
//======================
void PinHoleCamera::disp(){
	std::cout<<get_pin_hole_cam_string();
}
//======================
/*Ray PinHoleCamera::Pixel2Ray( int Uu, int Vv ){
	disp();

	int u =  Uu;
	int v =  Vv;

	std::cout<<"Pixel2Ray "<<Uu<<","<<Vv<<" -> "<<u<<","<<v<<"\n";

	return cam_send_ray( Vv, Uu );
}*/
//======================
CameraRay PinHoleCamera::cam_send_ray( int Uu, int Vv ){
	//calculate pixel coordinates
	int u = Uu-(SensorResolutionV/2);
	int v = Vv-(SensorResolutionU/2);
	//std::cout<<"image plane frame: ("<<u<<"|"<<v<<"), "<<std::endl;
	//std::cout<<"u: "<<u<<" v: "<<v<<std::endl;
	// calculate intersection of ray and image sensor
	//std::cout<<"dir u: "<<SensorDirectionU.get_string()<<std::endl;
	//std::cout<<"dir v: "<<SensorDirectionV.get_string()<<std::endl;
	Vector3D vec_intersection_ray_and_sensor =
		PrincipalPointOfSensorPlane+
		SensorDirectionU*u+
		SensorDirectionV*v;
	//std::cout<<"intersection: "<<vec_intersection_ray_and_sensor.get_string()<<std::endl;
	// calculate ray dir 
	Vector3D vec_ray_dir =  
	vec_intersection_ray_and_sensor - CameraPositionInWorld;
	// return
	CameraRay camera_ray;
	camera_ray.SetRay(CameraPositionInWorld,vec_ray_dir);
	return camera_ray;
}
//==================================================================
/*void PinHoleCamera::Acquire(		
	CartesianFrame &world,
	GlobalSettings &settings,
	CameraImage &ImagE
){
	uint i,u,v;
	cv::Vec3b intensity;
	CameraRay cam_ray;
	ColourProperties imag_col;

	#pragma omp parallel shared(settings,world) private(i,cam_ray,imag_col,intensity,u,v) 
	{	
		#pragma omp for schedule(dynamic) 
		for (i = 0; i < SensorResolutionUTimesV; i++) {
			
			u = i/SensorResolutionU;
			v = i%SensorResolutionU;
			//out<<i<<",";//<<"("<<u<<"|"<<v<<"), ";
			cam_ray = cam_send_ray(u,v);
			
			imag_col = cam_ray.trace(world,0,NULL,settings);
			// BGR colour code
		
			intensity.val[0]=imag_col.get_blue();
			intensity.val[1]=imag_col.get_green();
			intensity.val[2]=imag_col.get_red();

			//CameraImage->at<cv::Vec3b>(u,v) = intensity;
			ImagE.at<cv::Vec3b>(u,v) = intensity;
		}
	}
}*/
//==================================================================
void PinHoleCamera::cam_acquire_image_parallel(
	CartesianFrame* world,
	GlobalSettings* settings
){

	// Fork a team of threads giving them their own copies of
	// variables to fight the pixels !

	//==============================================================
	// parallel !!! FIRE !!!
	//==============================================================
	uint i,u,v;
	cv::Vec3b intensity;
	CameraRay cam_ray;
	ColourProperties imag_col;

	#pragma omp parallel shared(settings,world) private(i,cam_ray,imag_col,intensity,u,v) 
	{	
		#pragma omp for schedule(dynamic) 
		for (i = 0; i < SensorResolutionUTimesV; i++) {
			
			u = i/SensorResolutionU;
			v = i%SensorResolutionU;
			//out<<i<<",";//<<"("<<u<<"|"<<v<<"), ";
			cam_ray = cam_send_ray(u,v);
			

			imag_col = cam_ray.trace(world,0,NULL,settings);
			// BGR colour code
		
			intensity.val[0]=imag_col.get_blue();
			intensity.val[1]=imag_col.get_green();
			intensity.val[2]=imag_col.get_red();

			//CameraImage->at<cv::Vec3b>(u,v) = intensity;
			Image->at<cv::Vec3b>(u,v) = intensity;
		}
	}
	//==============================================================
	// All threads join master thread and disband
	//==============================================================
}
//==================================================================
void PinHoleCamera::cam_acquire_stereo_anaglyph_image(
	CartesianFrame* world,
	GlobalSettings* settings,
	double cmaera_offset_in_m
){
	
	if(cmaera_offset_in_m <= 0.0){
		cmaera_offset_in_m = 0.1;
		std::cout<<"PinHoleCam-> stereo -> offset was set to ";
		std::cout<<cmaera_offset_in_m<<" because it was zero ";
		std::cout<<"or negative!"<<std::endl;
	}
	
	
	Vector3D left_camera_position;
	Vector3D left_camera_pointing_direction;
	
	Vector3D right_camera_position;
	Vector3D right_camera_pointing_direction;
	
	calculate_stereo_parameters(world,settings,cmaera_offset_in_m,
	left_camera_position,
	left_camera_pointing_direction,
	right_camera_position,
	right_camera_pointing_direction);

	//==================================================================	
	// taking two images for stereo vision
	//==================================================================
	
	PinHoleCamera PinHoleCam_left;
	PinHoleCamera PinHoleCam_right;

	PinHoleCam_left.set_cam(
	"left_Cam",
	left_camera_position,
	CameraOrientationInWorld,
	SensorResolutionU,
	SensorResolutionV);

	PinHoleCam_right.set_cam(
	"right_Cam",
	right_camera_position,
	CameraOrientationInWorld,
	SensorResolutionU,
	SensorResolutionV);

	PinHoleCam_left.set_pin_hole_cam(FieldOfView_in_Rad);
	PinHoleCam_right.set_pin_hole_cam(FieldOfView_in_Rad);
	
	//==============================================================	
	// left
	//==============================================================
	
	PinHoleCam_left.set_pointing_direction(
	left_camera_pointing_direction,
	this->get_image_upwards_direction_in_world_frame());
	
	//~ PinHoleCam_left.disp();
	
	PinHoleCam_left.
	cam_acquire_image_parallel(world,settings);
	
	cv::Mat left_image = PinHoleCam_left.get_image();
	//==============================================================	
	// right
	//==============================================================	
	
	PinHoleCam_right.set_pointing_direction(
	right_camera_pointing_direction,
	this->get_image_upwards_direction_in_world_frame());
	
	//~ PinHoleCam_right.disp();
	
	PinHoleCam_right.
	cam_acquire_image_parallel(world,settings);
	
	cv::Mat right_image = PinHoleCam_right.get_image();							
//==================================================================	
// mix images
//==================================================================	
	// BGR
	CameraImage stereo_image;
	stereo_image.allocate_memory_for_image(SensorResolutionU,SensorResolutionV);	
	
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
	
	cv::merge(anaglyph_image_channels,*Image);
					
	//~ std::cout<<"PinHoleCam-> stereo -> end"<<std::endl;
}
//======================
