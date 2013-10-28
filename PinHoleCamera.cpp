#include "PinHoleCamera.h"
//======================
PinHoleCamera::PinHoleCamera(){
	CameraImageAlreadyAllocated=false;
}
//======================
PinHoleCamera::~PinHoleCamera(){
	delete CameraImage;
}
//======================
void PinHoleCamera::set_pin_hole_cam(double fov){
	//set field of view
	FieldOfView_in_Rad=fov;
	// calculate principal point (intersection of optical axis and
	// image sensor plane)
	// calculate distance of camera base and principal point
	// using FieldOfView_in_Rad angle 
	//
	//	         camera base
	//	              /|  \
	//	             / |  |
	//	            /  |  |
	//	           /   |  |
	//	          /    |  |
	//	         /fov/2|  |
	//	        /______|  |
	//	       /       |   \
	//	      /        |   /dist_camera_base_2_principal_point
	//	     /         |  |
	//	    /          |  |
	//	   /       ____|  |
	//	  /       | o  |  |
	//	 /________|____|  /
	//	        principal_point
	//	\______  ______/
	//	       \/
	//	      Uu/2
	//
	// distance
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
	// allocate ram for image
	if(CameraImageAlreadyAllocated){
	}else{
		std::cout<<"PinHoleCamera->allocate image memory"<<std::endl;
		CameraImage =new cv::Mat (SensorResolutionV,SensorResolutionU, CV_8UC3);
		CameraImageAlreadyAllocated=true;
	}
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
Ray PinHoleCamera::cam_send_ray(int Uu,int Vv){
	//FAST
	//calculate pixel coordinates
	int u = Uu-(SensorResolutionV/2);
	int v = Vv-(SensorResolutionU/2);
	//std::cout<<"image plane frame: ("<<u<<"|"<<v<<"), "<<std::endl;
	//std::cout<<"u: "<<u<<" v: "<<v<<std::endl;
	// calculate intersection of ray and image sensor
	//std::cout<<"dir u: "<<SensorDirectionU.get_string()<<std::endl;
	//std::cout<<"dir v: "<<SensorDirectionV.get_string()<<std::endl;
	Vector3D vec_intersection_ray_and_sensor;
	vec_intersection_ray_and_sensor=
	PrincipalPointOfSensorPlane+
	SensorDirectionU*u+
	SensorDirectionV*v;
	//std::cout<<"intersection: "<<vec_intersection_ray_and_sensor.get_string()<<std::endl;
	// calculate ray dir 
	Vector3D vec_ray_dir;
	vec_ray_dir = vec_intersection_ray_and_sensor - CameraPositionInWorld;
	// return
	Ray camera_ray;
	camera_ray.set_ray(CameraPositionInWorld,vec_ray_dir);
	return camera_ray;
}
//======================
void PinHoleCamera::cam_acquire_image_parallel(
CartesianFrame* world,
GlobalSettings* settings
){
	//init open mp variables
	//int number_of_threads, thread_id;
	
	std::cout <<"PinHoleCamera->acquire_image_parallel()"<<std::endl;
	// Fork a team of threads giving them their own copies of
	// variables to fight the pixels !

	//==============================================================
	// parallel !!! FIRE !!!
	//==============================================================
	int i,u,v;
	cv::Vec3b intensity;
	Ray cam_ray;
	ColourProperties imag_col;

	#pragma omp parallel shared(settings,world) private(i,cam_ray,imag_col,intensity,u,v) 
	{	
		// Obtain thread id 
		//thread_id = omp_get_thread_num();
		// Obtain number of threads avaliable 
		//if(thread_id == 0)
		//{number_of_threads = omp_get_num_threads();}

		//std::stringstream out; out.str("");
		//out<<"Hello from thread = ";
		//out<<(thread_id+1)<<" of "<< number_of_threads<<std::endl;
		
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

			CameraImage->at<cv::Vec3b>(u,v) = intensity;
		}
	}
	//==============================================================
	// All threads join master thread and disband
	//==============================================================
}
//==================================================================
void PinHoleCamera::cam_save_image(std::string str_image_name){
	// modify filename
	str_image_name += ".png";
	
	std::vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(1);			
	
	try {
		cv::imwrite(str_image_name, *CameraImage, compression_params);
	}
	catch (std::runtime_error& ex) {
		fprintf(stderr,"Exception converting image to PNG format: %s\n", ex.what());
	}
}
//======================
void PinHoleCamera::cam_acquire_image(
CartesianFrame* world,
GlobalSettings *settings,
std::string str_image_name
){
	// modify filename
	str_image_name += ".png";
	
	// init image
	cv::Mat image(SensorResolutionV,SensorResolutionU, CV_8UC3);
	std::vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);	
		
	std::stringstream out; out.str("");
	out<<"__________________________________________________"<<std::endl;
	out<<"| Tracing "<<str_image_name<<std::endl;
	out<<"| using seriall mode with pin-hole-cam model."<<std::endl;
	//==============================================================
	// tracing
	//==============================================================
	//#pragma omp parallel private(number_of_threads, thread_id)
	{
		int u; int v;
		cv::Vec3b intensity;
		Ray cam_ray;
		ColourProperties imag_col;
		for (int pixel_itterator = 0; 
			pixel_itterator < SensorResolutionUTimesV;
			pixel_itterator++) {
			
			u = pixel_itterator/SensorResolutionU;
			v = pixel_itterator%SensorResolutionU;
			//std::cout<<pixel_itterator<<"("<<u<<"|"<<v<<"), ";
			cam_ray = cam_send_ray(u,v);
			//cam_ray.disp();
			imag_col = cam_ray.trace(world,0,NULL,settings);
			// BGR
			intensity.val[0]=imag_col.get_blue();
			intensity.val[1]=imag_col.get_green();
			intensity.val[2]=imag_col.get_red();
			image.at<cv::Vec3b>(u,v) = intensity;
			//imag_col;
			//intensity.
		}

	}  
	//==============================================================
	// save image
	//==============================================================
	try {
		cv::imwrite(str_image_name, image, compression_params);
	}
	catch (std::runtime_error& ex) {
		fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
	}
	out<<"|_________________________________________________"<<std::endl;
	out<<std::endl;
	std::cout<<out.str();
}
//======================
cv::Mat PinHoleCamera::get_image()const{
	return *CameraImage;
}
