#include "LightSource.h"
//======================================================================
void LightSource::set_opening_angle(double new_OpeningAngle_in_rad){
	if(new_OpeningAngle_in_rad > 0.0)
	{
		OpeningAngle_in_rad=new_OpeningAngle_in_rad;
	}else{
		std::stringstream out;
		out<<"LightSource -> set_opening_angle: ";
		out<<new_OpeningAngle_in_rad<<" [rad] is not valid!";
		out<<" Opening_angle";
		out<<" has to be bigger than 0.0 [rad]."<<std::endl;
		std::cout<<out.str();
	}
}
//======================================================================
void LightSource::radiate(
CartesianFrame* world,
GlobalSettings* settings)const{	
	// When "radiate" is called the light source emitts rays in the
	// direction of the camera. The direction is distributed within the 
	// opening angel theta.
	
	Vector3D vec_dir_of_emitted_ray;
	// in its own frame the light bulb emmits in z direction.
	
	double dbl_theta_in_rad;
	double dbl_phi_in_rad;
	//r = 1.0 because the direction is normalized to 1.0
	
	// mt19937 is a standard mersenne_twister_engine
	std::mt19937 RNG_mt19937 
	(settings->get_seed_for_random_number_generator_mt19937()); 
		 	
	Ray ray_emitted_ray;
	for( int i=0; i<SensorResolutionUTimesV; i++)
	{	

		//cout << "Random value: ";
		//cout<<(double)RNG_mt19937()/(double)RNG_mt19937.max()<< std::endl;

		// random generation of emission angles
		// the sqrt() is necessary to spread the rays equally on the 
		// unit sphere. Otherwise there is a accumulation in the primary
		// emission direction
		//(double)rand()/(double)RAND_MAX
		dbl_theta_in_rad = OpeningAngle_in_rad *
		sqrt( 	(double)RNG_mt19937()/(double)RNG_mt19937.max()	);
							
	
		dbl_phi_in_rad =  2.0 *M_PI *
		(double)RNG_mt19937()/(double)RNG_mt19937.max();		
		
		vec_dir_of_emitted_ray.set(	
							sin(dbl_theta_in_rad)*cos(dbl_phi_in_rad),
							sin(dbl_theta_in_rad)*sin(dbl_phi_in_rad),
							cos(dbl_theta_in_rad)
							);
		
		//vec_dir_of_emitted_ray.disp();				
		T_Camera2World.transform_orientation(&vec_dir_of_emitted_ray);				
		//vec_dir_of_emitted_ray.disp();
		ray_emitted_ray.SetSupport(CameraPositionInWorld);
		ray_emitted_ray.SetDirection(vec_dir_of_emitted_ray);
		
		//ray_emitted_ray.trace_science(world,0,NULL,settings,0.0);
		//ray_emitted_ray.disp();
	}
}
//======================================================================
std::string LightSource::get_bulb_string()const{
		std::stringstream out; out.str("");
		out<<get_cam_string();
		out<<"||field of view: "<<OpeningAngle_in_rad<<" [rad] ";
		out<<"/ "<<(OpeningAngle_in_rad*360.0)/(2.0*M_PI);
		out<<" [deg]"<<std::endl;
		out<<"||Number of emitted rays: "<<SensorResolutionUTimesV<<std::endl;
		return out.str();
	}
//======================================================================
void LightSource::disp_bulb()const{
		std::cout<<get_bulb_string();
	}
//======================================================================
