#include "ApertureCamera.h"
//======================
bool ApertureCamera::set_object_distance(double n_object_distance){
	if(n_object_distance > 0.0)
	{
		// init object distance
		ObjectDistance_in_m = n_object_distance;
		// init sensor distance using the thin lense equation
		// 1/f = 1/b + 1/g
		// f=FocalLength_in_m
		// b=SensorDistance_in_m
		// g=ObjectDistance_in_m
		SensorDistance_in_m = 1.0/
		(1.0/FocalLength_in_m - 1.0/ObjectDistance_in_m);
	}else{
		std::stringstream out; out.str("");
		out<<"Object distance can not be negative!";
		out<<" ObjectDistance_in_m = ";
		out<<n_object_distance<<std::endl;
		std::cout<<out.str();
		return false;
	}
	return true;
}
//======================
bool ApertureCamera::set_aperture_cam(	
double new_FStopNumber,
double new_FocalLength,
double new_SensorSizeX,
double new_SensorSizeY,
int new_NumberOfRaysPerPixelToBeEmitted
){
	//======================
	// set FStopNumber
	//======================
	if(new_FStopNumber > 0.0){
		FStopNumber = new_FStopNumber;
	}else{
		std::stringstream out; out.str("");
		out<<"Focal ratio / f-stop number must not be negative!";
		out<<" FStopNumber = ";
		out<<new_FStopNumber<<std::endl;
		std::cout<<out.str();
		return false;
	}
	//======================
	// set FocalLength_in_m
	//======================
	if( new_FocalLength > 0.0){
		FocalLength_in_m =  new_FocalLength;
		// init aperture radius
		ApertureRadius_in_m = (FocalLength_in_m/FStopNumber)/2.0;
	}else{
		std::stringstream out; out.str("");
		out<<"Focal length can not be negative!";
		out<<" FocalLength_in_m = ";
		out<<new_FocalLength<<std::endl;
		std::cout<<out.str();
		return false;
	}
	//======================
	// set sensor size in x
	//======================
	if(new_SensorSizeX > 0.0){
		SensorSizeX_in_m = new_SensorSizeX;
	}else{
		std::stringstream out; out.str("");
		out<<"Sensor size in x can not be negative!";
		out<<" SensorSizeX_in_m = ";
		out<<new_SensorSizeX<<std::endl;
		std::cout<<out.str();
		return false;
	}
	//======================
	// set sensor size in y
	//======================
	if(new_SensorSizeY > 0.0){
		SensorSizeY_in_m = new_SensorSizeY;
	}else{
		std::stringstream out; out.str("");
		out<<"Sensor size in y can not be negative!";
		out<<" SensorSizeY_in_m = ";
		out<<new_SensorSizeY<<std::endl;
		std::cout<<out.str();
		return false;
	}
	//======================
	// set resolution
	//======================
		
		// the y resolution is given due to the x y sensor size
		// ratio
		SensorResolutionV = SensorSizeY_in_m/SensorSizeX_in_m*
		SensorResolutionU ;
		// init pixel pitch
		PixelPitch_in_m = SensorSizeX_in_m/double(SensorResolutionU);

	//======================
	// set number of rays per pixel to be emitted
	//======================
	if(new_NumberOfRaysPerPixelToBeEmitted > 0.0){
		NumberOfRaysPerPixelToBeEmitted = new_NumberOfRaysPerPixelToBeEmitted;
	}else{
		std::stringstream out; out.str("");
		out<<"Number of rays per pixel can not be negative!";
		out<<" NumberOfRaysPerPixelToBeEmitted = ";
		out<<new_NumberOfRaysPerPixelToBeEmitted<<std::endl;
		std::cout<<out.str();
		return false;
	}
	//======================
	// init default object focus 
	//======================
	// set focus to infinity (a big value compared to focal length)
	set_object_distance(FocalLength_in_m*1000000.0);

	// init back transformation T_cam2world
	T_World2Camera = T_Camera2World.inverse();
	//======================
	// init pseudo random number generator
	//======================
	srand((unsigned)time(0));
	
	//======================
	// init image resolution
	//======================
	SensorResolutionUTimesV = SensorResolutionU * SensorResolutionV;
	return true;
}
//======================
std::string ApertureCamera::get_aperture_cam_string(){
	std::stringstream out; out.str("");
	out<<"||Focal ratio      : ";
	out<<(FStopNumber);
	out<<" [1]"<<std::endl;	
	out<<"||Focal length     : ";
	out<<(FocalLength_in_m)*1000.0;
	out<<" [mm]"<<std::endl;	
	out<<"||Aperture diameter: ";
	out<<(FocalLength_in_m/FStopNumber)*1000.0;
	out<<" [mm]"<<std::endl;	
	out<<"||Sensor size      : ";
	out<<1000.0*SensorSizeX_in_m<<" x "<<1000.0*SensorSizeY_in_m;
	out<<" [mm^2]"<<std::endl;	
	out<<"||Sensor resolution: "<<SensorResolutionU<<" x ";
	out<<SensorResolutionV<<" [pixels] / ";
	out<<double(SensorResolutionV * SensorResolutionU)/1000000.0;
	out<<" [M pixels]"<<std::endl;
	out<<"||Pixel pitch      : "<<PixelPitch_in_m*1000000;
	out<<" [um]"<<std::endl;
	out<<"||Object distance  : "<<ObjectDistance_in_m<<" [m]"<<std::endl;
	out<<"||Sensor distance  : "<<SensorDistance_in_m<<" [m]"<<std::endl;
	out<<"||Rays per pixel   : ";
	out<<NumberOfRaysPerPixelToBeEmitted<<std::endl;
	out<<"||Rays per image   : ";
	out<<double(NumberOfRaysPerPixelToBeEmitted*
	SensorResolutionV*SensorResolutionU)/1000000<<" [M rays]"<<std::endl;
	//out<<mirror.get_parabolic_round();
	return out.str();
}
//======================
void ApertureCamera::disp(){
	std::cout<<get_cam_string();
	std::cout<<get_aperture_cam_string();
}
//======================
void ApertureCamera::cam_send_ray
(double Xx,double Yy,Ray* ray_camera_pixel_x_y){

	//======================
	//calculate pixel coordinates
	//======================
	int x;
	int y;
	x = -Xx+(SensorResolutionV/2);
	y = -Yy+(SensorResolutionU/2);
	
	Vector3D vec_pixel_x_y;
	vec_pixel_x_y.set_vec3D(
	//x
	x*PixelPitch_in_m,
	//y
	y*PixelPitch_in_m,
	//z
	0.0
	);
	
	//======================
	//calculate lens intersection
	//======================
	double r;
	double phi;
	r = (double)rand()/(double)RAND_MAX * ApertureRadius_in_m;
	phi = (double)rand()/(double)RAND_MAX * 2.0 * M_PI;
	//std::cout<<"r: "<<r<<" phi: "<<phi<<std::endl;
	
	Vector3D vec_lens_inersection;
	vec_lens_inersection.set_vec3D(
	//x
	r*cos(phi),
	//y
	r*sin(phi),
	//z
	SensorDistance_in_m
	);
	
	//======================
	//calculate object plane intersection
	//======================
	Vector3D vec_object_plane_intersection;
	vec_object_plane_intersection.set_vec3D(
	//x
	-x*PixelPitch_in_m * ObjectDistance_in_m/SensorDistance_in_m,
	//y
	-y*PixelPitch_in_m * ObjectDistance_in_m/SensorDistance_in_m,
	//z
	ObjectDistance_in_m
	);
	
	//======================
	// calculate ray direction
	//======================
	Vector3D vec_dir_ray;
	vec_dir_ray= 
	vec_object_plane_intersection-vec_lens_inersection;
	
	//======================
	// init ray
	//======================
	// in camera frame
	// base: vec_lens_inersection
	// dir : vec_dir_ray
	
	// base in world frame
	T_Camera2World.transform_orientation(&vec_lens_inersection);
	
	Vector3D vec_ray_base;
	vec_ray_base = CameraPositionInWorld + vec_lens_inersection;
	
	// dir in world frame
	T_Camera2World.transform_orientation(&vec_dir_ray);
	
	ray_camera_pixel_x_y->set_ray(vec_ray_base,vec_dir_ray);
}
//======================
void ApertureCamera::cam_acquire_image(
CartesianFrame* world,
GlobalSettings* settings,
std::string str_image_file_name
){
	// modify filename
	str_image_file_name += ".png";
	
	int thread_id;
	
	// init image
	cv::Mat image(SensorResolutionV,SensorResolutionU, CV_8UC3);
	std::vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);		
	
	//std::cout <<"Hello from "<<CameraName<<" in non parallel mode:"<<std::endl;
	// Fork a team of threads giving them their own copies of
	// variables to fight the pixels !

	//==============================================================
	// parallel !!! FIRE !!!
	//==============================================================
	double WeightOfSingleRayForColourMixture = 
	1.0/double(NumberOfRaysPerPixelToBeEmitted);
	
	double u; double v;
	cv::Vec3b intensity;
	Ray cam_ray;
	ColourProperties imag_col;
	ColourProperties col_of_single_ray;
	int i;
	int in_ray_count;
	
	#pragma omp parallel shared(image,settings,world) private(i,in_ray_count,cam_ray,imag_col,col_of_single_ray,intensity,u,v,thread_id) 
	{	
		// Obtain thread id 
		//thread_id = omp_get_thread_num();
		// Obtain number of threads avaliable 
		//number_of_threads = omp_get_num_threads();
		
		//int  part_line = SensorResolutionV/number_of_threads;
		//int start_line = part_line*(thread_id);
		//int   end_line = part_line*(thread_id+1)-1;

		//std::stringstream out; out.str("");
		//out<<"Hello World from single thread.";
		//out<<(thread_id+1)<<" of "<< number_of_threads<<std::endl;
		//out<<"I will work on the pixels: ";
		
		#pragma omp for schedule(dynamic) 
		for (i = 0; i < SensorResolutionUTimesV; i++) 
		{
			u = double(i/SensorResolutionU);
			v = double(i%SensorResolutionU);
			//std::cout<<i<<"("<<u<<"|"<<v<<"), ";
			//std::cout<<WeightOfSingleRayForColourMixture<<std::endl;
			imag_col.set_colour_0to255(0,0,0);
			//std::cout<<"single pixel"<<std::endl;
			for(in_ray_count=0;
			 in_ray_count<NumberOfRaysPerPixelToBeEmitted;
			  in_ray_count++)
				{
					
				cam_send_ray(u,v,&cam_ray);
			
				//cam_ray.disp();
				col_of_single_ray = cam_ray.trace(world,0,NULL,settings);
				//col_of_single_ray.disp();
				imag_col.mixture(&col_of_single_ray,WeightOfSingleRayForColourMixture);
				//imag_col.disp();
				}
			// BGR
			intensity.val[0]=imag_col.get_blue();
			intensity.val[1]=imag_col.get_green();
			intensity.val[2]=imag_col.get_red();
			image.at<cv::Vec3b>(int(u),int(v)) = intensity;
		}
	}  
	//==============================================================
	// All threads join master thread and disband
	//==============================================================
	// save image
	try {
		cv::imwrite(str_image_file_name, image, compression_params);
	}
	catch (std::runtime_error& ex) {
		fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
	}
}
