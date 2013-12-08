//======================================================================
// Author: 	Sebastian Mueller 
// 		Department of Physics 
//		University of Dortmund in Germany
//		Chair e5b supervised by Prof. Dr. Dr. Wolfgang Rhode
//		year 2013
// Title:	
//			Tracer is a raytracing rendering programm made by the author
//			in his freetime.
//			
//			A rendering algorithm transforms 3D objects and light 
//			phenomena into a 2D plane, aka an image.
//
//			Tracer was inspired by all the beautifull light 
//			phenomenas in nature which are visible by the human eye 
//			while riding ones bikecycle.
//		
//			This version of tracer is the first cpp implementation.
//			When you are new to raytracing consider to use "pov ray" to
//			fit your wishes.
//			Tracer is much less powerfull by means of varity of the 
//			supported features. 
//			It is only used because the author likes it and is known to
//			it. But anyway the authuor invites you to make tracer a 
//			powerfull tool in scientific research.
//			 
// 			This version was made to investigate optical devices like
//			Telescopes.
//			Espacially the Cherenkov Light FACT-Telescope on La Palma,
//			Spain
//			
//	features:
//			Tracer supports a tree structure to store its 3D objects.
//			A pre-tracer is used to generate a list of hit candidates
//			using a maximum norm calculation of each pixel.
//			This is the most important feature which gains a speed up
//			in the number of objects n in a scene by
//			O(n^1) to O(log(n)).
//  compile:
//			cmake .
//			make
//
//======================================================================

// std
#include <iostream> 
#include <string>
#include <sstream>
#include <random>

#include "WorldFactory.h"
#include "SensorIntersection.h"
#include "ReflectionProperties.h"
#include "ColourProperties.h"
#include "GlobalSettings.h"
#include "Vector3D.h"
#include "Rotation3D.h"
#include "HomoTrafo3D.h"
#include "Intersection.h"
#include "CartesianFrame.h"
#include "SurfaceEntity.h"
#include "Ray.h"
#include "CameraDevice.h"
#include "PinHoleCamera.h"
#include "ApertureCamera.h"
#include "LightSource.h"
#include "FreeOrbitCamera.h"
// geometry
#include "Triangle.h"
#include "Plane.h"
#include "Disc.h"
#include "SensorDisc.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "OpticalMirror.h"
#include "OpticalMirrorRound.h"
#include "OpticalMirrorHexagonal.h"
#include "OpticalMirrorSphericRound.h"
#include "OpticalMirrorSphericHexagonal.h"
#include "OpticalMirrorParabolicRound.h"
#include "OpticalMirrorParabolicHexagonal.h"
#include "OpticalMirrorEllipsoidHexagonal.h"

#include "FactTelescope.h"

// namespaces
using namespace cv;
using namespace std;

// friends of osstream
//======================================================================
std::ostream& operator<<(std::ostream& os, const Vector3D& vec){
    os << "("<<vec.get_x()<<"|"<<vec.get_y()<<"|"<<vec.get_z()<<")";
    os << "[m]";
    return os;
}
//======================================================================
std::ostream& operator<<(std::ostream& os, const Rotation3D& rot){
    os << "("<<rot.get_rot_x()<<"|";
    os <<      rot.get_rot_y()<<"|";
    os <<      rot.get_rot_z()<<")";
    os << "[rad]";
    return os;
}
//======================================================================
std::ostream& operator<<(std::ostream& os, const ColourProperties& col){
    os << "("<<int(col.get_red())<<"|";
    os <<      int(col.get_green())<<"|";
    os <<      int(col.get_blue())<<")";
    os << "[RGB 8bit]";
    return os;
}
//======================================================================
std::ostream& operator<<(std::ostream& os, const ReflectionProperties& refl){
    
    if(refl.get_reflection_flag()) {
		os << "refl. coeff.=";
		os << refl.get_reflection_coefficient();
	}else{
		os << "FALSE";
	}
	return os;
}
//======================================================================
// main
//======================================================================
int main(){
	//==================================================================
	// start tracer
	//==================================================================
	int system_call_return_value = system("clear");
	system_call_return_value = system("clear");
	system_call_return_value = system("clear");
	
	stringstream out;
	out.str("");
	out<<" ____________________________________________________ "<<endl;
	out<<"|                                                    |"<<endl;
	out<<"|               welcome to MCtracer                  |"<<endl;
	out<<"|                                                    |"<<endl;
	out<<"|   high performance raytracing tool to design and   |"<<endl;
	out<<"|  investigate optical devices for particle physics  |"<<endl;
	out<<"|                                                    |"<<endl;
	out<<"|  Author: Sebastian Mueller              year 2013  |"<<endl;
	out<<"|____________________________________________________|"<<endl;
	cout<<out.str();
	//==================================================================
	// random number seed
	// only call once !
	//==================================================================
	srand(time(0));
	
	//==================================================================
	// set up global settings
	//==================================================================
	GlobalSettings settings;
	settings.set_max_number_of_reflections(3);
	
	//==================================================================
	// open / read file
	//==================================================================
	WorldFactory file2world;
	
	string user_input;
	cout << "Enter a file to load: ";
	cin  >> user_input;
	
	bool loading_file_was_successful = file2world.load_file(user_input);
	
	if(loading_file_was_successful){
		
		CartesianFrame *Mworld = file2world.get_pointer_to_world();

		//cout << Mworld->get_frame_prompt_including_children();
		
		FreeOrbitCamera free;
		free.set_free_orbit(Mworld,&settings);
		free.start_free_orbit();
	}

	//==================================================================
	// decleration of objects;
	//==================================================================
	//int number_of_alphas = 101;
	//for(int alpha_itterator=0;alpha_itterator<number_of_alphas;alpha_itterator++)
	//{	
		//~ Vector3D pos;
		//~ Rotation3D rot;
		//~ pos.set_vec3D(0.0,0.0,0.0);
		//~ rot.set_rotation(0.0,0.0,0.0);
		//~ CartesianFrame   	world;
		//~ world.set_frame("world",pos,rot);
		//~ 
		//~ CartesianFrame 	house;
		//~ pos.set_vec3D(5.0,0.0,2.7);
		//~ rot.set_rotation(0.0,0.0,-((M_PI*2.0)/360.0)*0.0);
		//~ house.set_frame("house",pos,rot);	
		//~ 
		//~ ColourProperties col; 	
		//~ ReflectionProperties refl;		
//~ 
		//~ Plane 	floor;
		//~ pos.set_vec3D(0.0,0.0,0.0);
		//~ rot.set_rotation(0.0,0.0,((M_PI*2.0)/360.0)*0.0);
		//~ floor.set_frame("floor",pos,rot);
		//~ col.set_colour_0to255(0,255,0);
		//~ refl.set_reflection_coefficient(0.25);	
		//~ floor.set_surface_properties(&refl,&col);
		//~ floor.set_plane(-1.0,1.0,-1.0,1.0);	
			//~ 
		//~ //Vector3D rot_axis; rot_axis.set_vec3D(0.0,1.0,0.0);
		//~ //double rot_angle = (M_PI*2.0)/360*45.0;
		//~ Plane 	wall;
		//~ pos.set_vec3D(0.0,0.0,-0.0);
		//~ rot.set_rotation(0.0,(M_PI*2.0/360.0)*90.0,(M_PI*2.0/360.0)*30.0);
		//~ //rot.set_rotation(rot_axis,rot_angle);
		//~ wall.set_frame("wall",pos,rot);
		//~ col.set_colour_0to255(0,0,255);
		//~ refl.set_reflection_coefficient(0.233);	
		//~ wall.set_surface_properties(&refl,&col);
		//~ wall.set_plane(-1.0,1.0,-1.0,1.0);
		//~ 
		//~ Sphere 	ball;
		//~ pos.set_vec3D(0.0,0.0,0.0);
		//~ rot.set_rotation(0.0,0.0,0.0);
		//~ ball.set_frame("ball",pos,rot);
		//~ col.set_colour_0to255(255,0,0);
		//~ refl.set_reflection_coefficient(0.51);	
		//~ ball.set_surface_properties(&refl,&col);			
		//~ ball.set_sphere(0.5);
		//~ /*
		//~ OpticalMirrorSphericHexagonal spheric;
		//~ pos.set_vec3D(-0.5,-1.0,-2.5);
		//~ rot.set_rotation(0.0,0.0,0.0);
		//~ spheric.set_frame("spheric",pos,rot);
		//~ col.set_colour_0to255(0,0,255);
		//~ refl.set_reflection_coefficient(0.9);	
		//~ spheric.set_surface_properties(&refl,&col);			
		//~ spheric.set_spheric_hexag(1.5*4.89,1.0);
		//~ 
		//~ OpticalMirrorParabolicHexagonal para;
		//~ pos.set_vec3D(-0.5,+1.0,-2.5);
		//~ rot.set_rotation(0.0,0.0,-((M_PI*2.0)/360.0)*5.0);
		//~ para.set_frame("para",pos,rot);
		//~ col.set_colour_0to255(0,0,255);
		//~ refl.set_reflection_coefficient(0.9);	
		//~ para.set_surface_properties(&refl,&col);			
		//~ para.set_parabolic_hexag(1.5*4.89,1.0);
		//~ */
		//~ 
		//~ //crosshair
		//~ /*
		//~ double dbl_cross_rotation = ((M_PI*2.0)/360.0)*30.0;
		//~ 
		//~ Plane 	crosshair_hex_0;
		//~ pos.set_vec3D(0.0,0.0,4.889);
		//~ rot.set_rotation(0.0,0.0,dbl_cross_rotation);
		//~ crosshair_hex_0.set_frame("crosshair_hex_0",pos,rot);
		//~ col.set_colour_0to255(125,0,0);
		//~ refl.set_reflection_coefficient(0.0);	
		//~ crosshair_hex_0.set_surface_properties(&refl,&col);
		//~ crosshair_hex_0.set_plane(-0.195,0.195,-0.001,0.001);
		//~ 
		//~ Plane 	crosshair_hex_1;
		//~ pos.set_vec3D(0.0,0.0,4.889);
		//~ rot.set_rotation(0.0,0.0,((M_PI*2.0)/360.0)*60.0+dbl_cross_rotation);
		//~ crosshair_hex_1.set_frame("crosshair_hex_1",pos,rot);
		//~ col.set_colour_0to255(125,0,0);
		//~ refl.set_reflection_coefficient(0.0);	
		//~ crosshair_hex_1.set_surface_properties(&refl,&col);
		//~ crosshair_hex_1.set_plane(-0.195,0.195,-0.001,0.001);
		//~ 
		//~ Plane 	crosshair_hex_2;
		//~ pos.set_vec3D(0.0,0.0,4.889);
		//~ rot.set_rotation(0.0,0.0,((M_PI*2.0)/360.0)*120.0+dbl_cross_rotation);
		//~ crosshair_hex_2.set_frame("crosshair_hex_2",pos,rot);
		//~ col.set_colour_0to255(125,0,0);
		//~ refl.set_reflection_coefficient(0.0);	
		//~ crosshair_hex_2.set_surface_properties(&refl,&col);
		//~ crosshair_hex_2.set_plane(-0.195,0.195,-0.001,0.001);
		//~ 
		//~ Plane 	crosshair_hex_1_0;
		//~ pos.set_vec3D(0.0,0.0,4.889);
		//~ rot.set_rotation(0.0,0.0,((M_PI*2.0)/360.0)*30.0+dbl_cross_rotation);
		//~ crosshair_hex_1_0.set_frame("crosshair_hex_1_0",pos,rot);
		//~ col.set_colour_0to255(0,0,125);
		//~ refl.set_reflection_coefficient(0.0);	
		//~ crosshair_hex_1_0.set_surface_properties(&refl,&col);
		//~ crosshair_hex_1_0.set_plane(-0.195,0.195,-0.001,0.001);
		//~ 
		//~ Plane 	crosshair_hex_1_1;
		//~ pos.set_vec3D(0.0,0.0,4.889);
		//~ rot.set_rotation(0.0,0.0,((M_PI*2.0)/360.0)*90.0+dbl_cross_rotation);
		//~ crosshair_hex_1_1.set_frame("crosshair_hex_1_1",pos,rot);
		//~ col.set_colour_0to255(0,0,125);
		//~ refl.set_reflection_coefficient(0.0);	
		//~ crosshair_hex_1_1.set_surface_properties(&refl,&col);
		//~ crosshair_hex_1_1.set_plane(-0.195,0.195,-0.001,0.001);
		//~ 
		//~ Plane 	crosshair_hex_1_2;
		//~ pos.set_vec3D(0.0,0.0,4.889);
		//~ rot.set_rotation(0.0,0.0,((M_PI*2.0)/360.0)*150.0+dbl_cross_rotation);
		//~ crosshair_hex_1_2.set_frame("crosshair_hex_1_2",pos,rot);
		//~ col.set_colour_0to255(0,0,125);
		//~ refl.set_reflection_coefficient(0.0);	
		//~ crosshair_hex_1_2.set_surface_properties(&refl,&col);
		//~ crosshair_hex_1_2.set_plane(-0.195,0.195,-0.001,0.001);
		//~ */
//~ //======================================================================
		//~ 
		//~ double dbl_FACT_X_rotangle_in_grad=0.0;
		//~ double dbl_FACT_Y_rotangle_in_grad=0.0;
		//~ double dbl_FACT_Z_rotangle_in_grad=0.0;
		//~ 
		//~ double alpha_Davies_Cotton_parabola = 0.0;
		//~ 
		//~ cout<<alpha_Davies_Cotton_parabola<<endl;
		//~ FactTelescope fact(alpha_Davies_Cotton_parabola);
		//~ string telescope_system_name = "DC";
		//~ pos.set_vec3D(0.0,0.0,0.0);
		//~ rot.set_rotation(	((M_PI*2.0)/360.0)*dbl_FACT_X_rotangle_in_grad,
						//~ ((M_PI*2.0)/360.0)*dbl_FACT_Y_rotangle_in_grad,
						//~ ((M_PI*2.0)/360.0)*dbl_FACT_Z_rotangle_in_grad);
						//~ 
		//~ fact.set_frame("fact",pos,rot);
		//~ fact.init();
		//~ 
		//~ CartesianFrame GAPD_camera;
		//~ pos.set_vec3D(0.0,0.0,4.8892);
		//~ rot.set_rotation(0.0,0.0,0.0);
		//~ GAPD_camera.set_frame("GAPD_camera",pos,rot);			
//~ 
		//~ Disc 	Disc;
		//~ pos.set_vec3D(0.0,0.0,0.01);
		//~ rot.set_rotation(0.0,0.0,((M_PI*2.0)/360.0)*0.0);
		//~ Disc.set_frame("GAPD_camera_window",pos,rot);
		//~ col.set_colour_0to255(230,230,215);
		//~ refl.set_reflection_coefficient(0.0);	
		//~ Disc.set_surface_properties(&refl,&col);
		//~ Disc.set_Disc(0.33);
		//~ 
		//~ Disc 	Disc2;
		//~ pos.set_vec3D(0.0,0.0,0.89);
		//~ rot.set_rotation(0.0,0.0,((M_PI*2.0)/360.0)*0.0);
		//~ Disc2.set_frame("GAPD_camera_top",pos,rot);
		//~ col.set_colour_0to255(230,230,215);
		//~ refl.set_reflection_coefficient(0.0);	
		//~ Disc2.set_surface_properties(&refl,&col);
		//~ Disc2.set_Disc(0.33);
			//~ 
		//~ SensorDisc 	sensor_Disc;
		//~ pos.set_vec3D(0.0,0.0,0.0);
		//~ rot.set_rotation(0.0,0.0,0.0);
		//~ sensor_Disc.set_frame("GAPD_camera_sensor_Disc",pos,rot);
		//~ col.set_colour_0to255(0,255,128);
		//~ refl.set_reflection_coefficient(0.0);	
		//~ sensor_Disc.set_surface_properties(&refl,&col);
		//~ sensor_Disc.set_Disc(0.195);
		//~ //sensor_Disc.set_Disc(3.0);
	//~ 
		//~ SensorDisc 	garbage_esensor_Disc;
		//~ pos.set_vec3D(0.0,0.0,-1.0);
		//~ rot.set_rotation(0.0,0.0,0.0);
		//~ garbage_esensor_Disc.set_frame("garbage_esensor_Disc",pos,rot);
		//~ col.set_colour_0to255(155,128,128);
		//~ refl.set_reflection_coefficient(0.0);	
		//~ garbage_esensor_Disc.set_surface_properties(&refl,&col);
		//~ garbage_esensor_Disc.set_Disc(4.0);
		//~ //sensor_Disc.set_Disc(3.0);
	//~ 
		//~ Cylinder cam_cylinder;		
		//~ pos.set_vec3D(0.0,0.0,0.445);
		//~ rot.set_rotation(0.0,0.0,0.0);
		//~ cam_cylinder.set_frame("GAPD_camera_cylinder",pos,rot);
		//~ col.set_colour_0to255(230,230,215);
		//~ refl.set_reflection_coefficient(0.0);	
		//~ cam_cylinder.set_surface_properties(&refl,&col);
		//~ cam_cylinder.set_cylinder(0.33,0.89);		
		//~ //cylinder.disp();
//~ 
		//~ Vector3D vec_start; 
		//~ Vector3D vec_end  ; 
//~ 
		//~ Cylinder cylinder2;		
		//~ vec_start.set_vec3D(-1.65,-1.0,0.0);
		//~ vec_end.set_vec3D(-0.25,-0.25,4.9);
		//~ cylinder2.set_frame("cylinder2",pos,rot);
		//~ col.set_colour_0to255(230,75,75);
		//~ refl.set_reflection_coefficient(0.5);	
		//~ cylinder2.set_surface_properties(&refl,&col);
		//~ cylinder2.set_cylinder(0.024, vec_start, vec_end);		
		//~ //cylinder2.disp();
//~ 
		//~ CartesianFrame support_structure;
		//~ pos.set_vec3D(0.0,0.0,0.0);
		//~ rot.set_rotation(0.0,0.0,0.0);
		//~ support_structure.set_frame("support_structure",pos,rot);
		//~ 
		//~ Cylinder support_tube_1;		
		//~ vec_start.set_vec3D(1.5,1.0,0.0);
		//~ vec_end.set_vec3D(0.2,0.2,4.9);
		//~ support_tube_1.set_frame("support_tube_1",pos,rot);
		//~ col.set_colour_0to255(75,75,75);
		//~ refl.set_reflection_coefficient(0.0);	
		//~ support_tube_1.set_surface_properties(&refl,&col);
		//~ support_tube_1.set_cylinder(0.03, vec_start, vec_end);		
		//~ 
		//~ Cylinder support_tube_2;		
		//~ vec_start.set_vec3D(1.5,-1.0,0.0);
		//~ vec_end.set_vec3D(0.2,-0.2,4.9);
		//~ support_tube_2.set_frame("support_tube_2",pos,rot);
		//~ col.set_colour_0to255(75,75,75);
		//~ refl.set_reflection_coefficient(0.0);	
		//~ support_tube_2.set_surface_properties(&refl,&col);
		//~ support_tube_2.set_cylinder(0.03, vec_start, vec_end);	
		//~ 
		//~ Cylinder support_tube_3;		
		//~ vec_start.set_vec3D(-1.5,-1.0,0.0);
		//~ vec_end.set_vec3D(-0.2,-0.2,4.9);
		//~ support_tube_3.set_frame("support_tube_3",pos,rot);
		//~ col.set_colour_0to255(75,75,75);
		//~ refl.set_reflection_coefficient(0.0);	
		//~ support_tube_3.set_surface_properties(&refl,&col);
		//~ support_tube_3.set_cylinder(0.03, vec_start, vec_end);	
		//~ 
		//~ Cylinder support_tube_4;		
		//~ vec_start.set_vec3D(-1.5,1.0,0.0);
		//~ vec_end.set_vec3D(-0.2,0.2,4.9);
		//~ support_tube_4.set_frame("support_tube_3",pos,rot);
		//~ col.set_colour_0to255(75,75,75);
		//~ refl.set_reflection_coefficient(0.0);	
		//~ support_tube_4.set_surface_properties(&refl,&col);
		//~ support_tube_4.set_cylinder(0.03, vec_start, vec_end);	
//~ 
		//~ CartesianFrame support_structure2;
		//~ pos.set_vec3D(0.0,0.0,2.7);
		//~ rot.set_rotation(0.0,0.0,0.0);
		//~ support_structure2.set_frame("support_structure2",pos,rot);
		//~ 
		//~ Cylinder support_tube_1_1;		
		//~ vec_start.set_vec3D(-0.83,0.57,0.0);
		  //~ vec_end.set_vec3D(+0.83,0.57,0.0);
		//~ support_tube_1_1.set_frame("support_tube_1_1",pos,rot);
		//~ col.set_colour_0to255(75,75,75);
		//~ refl.set_reflection_coefficient(0.0);	
		//~ support_tube_1_1.set_surface_properties(&refl,&col);
		//~ support_tube_1_1.set_cylinder(0.015, vec_start, vec_end);	
		//~ 
		//~ Cylinder support_tube_1_2;		
		//~ vec_start.set_vec3D(-0.83,-0.57,0.0);
		  //~ vec_end.set_vec3D(+0.83,-0.57,0.0);
		//~ support_tube_1_2.set_frame("support_tube_1_2",pos,rot);
		//~ col.set_colour_0to255(75,75,75);
		//~ refl.set_reflection_coefficient(0.0);	
		//~ support_tube_1_2.set_surface_properties(&refl,&col);
		//~ support_tube_1_2.set_cylinder(0.015, vec_start, vec_end);
		//~ 
		//~ Cylinder support_tube_1_3;		
		//~ vec_start.set_vec3D(+0.83,+0.57,0.0);
		  //~ vec_end.set_vec3D(+0.83,-0.57,0.0);
		//~ support_tube_1_3.set_frame("support_tube_1_3",pos,rot);
		//~ col.set_colour_0to255(75,75,75);
		//~ refl.set_reflection_coefficient(0.0);	
		//~ support_tube_1_3.set_surface_properties(&refl,&col);
		//~ support_tube_1_3.set_cylinder(0.015, vec_start, vec_end);
	//~ 
		//~ Cylinder support_tube_1_4;		
		//~ vec_start.set_vec3D(-0.83,-0.57,0.0);
		  //~ vec_end.set_vec3D(-0.83,+0.57,0.0);
		//~ support_tube_1_4.set_frame("support_tube_1_4",pos,rot);
		//~ col.set_colour_0to255(75,75,75);
		//~ refl.set_reflection_coefficient(0.0);	
		//~ support_tube_1_4.set_surface_properties(&refl,&col);
		//~ support_tube_1_4.set_cylinder(0.015, vec_start, vec_end);
		//~ //support_tube_4.disp();	
		//~ //=================
		//~ // test ellipsoid
		//~ //=================
		//~ /*
		//~ OpticalMirrorEllipsoidHexagonal test_ellipsoid;
		//~ pos.set_vec3D(0.0,0.0,0.0);
		//~ rot.set_rotation(0.0,0.0,0.0);
		//~ test_ellipsoid.set_frame("test_ellipsoid",pos,rot);
		//~ col.set_colour_0to255(255,255,255);
		//~ refl.set_reflection_coefficient(0.9);
		//~ test_ellipsoid.set_surface_properties(&refl,&col);
		//~ test_ellipsoid.set_ellipsoid_hexag(4.99,4.79,0.35);
		//~ 
		//~ OpticalMirrorSphericHexagonal test_ellipsoid;
		//~ pos.set_vec3D(0.0,0.0,0.0);
		//~ rot.set_rotation(0.0,0.0,0.0);
		//~ test_ellipsoid.set_frame("test_ellipsoid",pos,rot);
		//~ col.set_colour_0to255(255,255,255);
		//~ refl.set_reflection_coefficient(0.9);
		//~ test_ellipsoid.set_surface_properties(&refl,&col);
		//~ test_ellipsoid.set_spheric_hexag(4.89,0.35);
		//~ 
		//~ double shift_in_m = +0.0;
		//~ Disc 	Disc_little_sensor;
		//~ pos.set_vec3D(0.0,0.0,4.891+shift_in_m);
		//~ rot.set_rotation(0.0,0.0,((M_PI*2.0)/360.0)*0.0);
		//~ Disc_little_sensor.set_frame("Disc_little_sensor",pos,rot);
		//~ col.set_colour_0to255(230,230,215);
		//~ refl.set_reflection_coefficient(0.0);	
		//~ Disc_little_sensor.set_surface_properties(&refl,&col);
		//~ Disc_little_sensor.set_Disc(0.03);
			//~ 
		//~ SensorDisc 	little_sensor_Disc;
		//~ pos.set_vec3D(0.0,0.0,4.89+shift_in_m);
		//~ rot.set_rotation(0.0,0.0,0.0);
		//~ little_sensor_Disc.set_frame("little_sensor_Disc",pos,rot);
		//~ col.set_colour_0to255(0,255,128);
		//~ refl.set_reflection_coefficient(0.0);	
		//~ little_sensor_Disc.set_surface_properties(&refl,&col);
		//~ little_sensor_Disc.set_Disc(0.025);
		//~ //little_sensor_Disc.set_Disc(3.0);
		//~ */
		//~ //==============================================================
		//~ // declare relationships
		//~ //==============================================================		
		//~ house.set_mother_and_child(&floor);
		//~ house.set_mother_and_child(&wall);
		//~ house.set_mother_and_child(&ball);
		//~ 
		//~ GAPD_camera.set_mother_and_child(&sensor_Disc);
		//~ GAPD_camera.set_mother_and_child(&Disc);
		//~ GAPD_camera.set_mother_and_child(&Disc2);
		//~ GAPD_camera.set_mother_and_child(&cam_cylinder);
		//~ 
		//~ support_structure.set_mother_and_child(&support_tube_1);
		//~ support_structure.set_mother_and_child(&support_tube_2);
		//~ support_structure.set_mother_and_child(&support_tube_3);
		//~ support_structure.set_mother_and_child(&support_tube_4);
		//~ support_structure2.set_mother_and_child(&support_tube_1_1);
		//~ support_structure2.set_mother_and_child(&support_tube_1_2);
		//~ support_structure2.set_mother_and_child(&support_tube_1_3);
		//~ support_structure2.set_mother_and_child(&support_tube_1_4);	
			//~ 
		//~ fact.set_mother_and_child(&support_structure);
		//~ fact.set_mother_and_child(&support_structure2);
		//~ fact.set_mother_and_child(&GAPD_camera);
		//~ /*
		//~ fact.set_mother_and_child(&crosshair_hex_0);
		//~ fact.set_mother_and_child(&crosshair_hex_1);
		//~ fact.set_mother_and_child(&crosshair_hex_2);
		//~ fact.set_mother_and_child(&crosshair_hex_1_0);
		//~ fact.set_mother_and_child(&crosshair_hex_1_1);
		//~ fact.set_mother_and_child(&crosshair_hex_1_2);
		//~ */
		//~ world.set_mother_and_child(&house);
		//~ world.set_mother_and_child(&garbage_esensor_Disc);
		//~ world.set_mother_and_child(&fact); 
		//~ /*
		//~ wo0rld.set_mother_and_child(&test_ellipsoid);
		//~ world.set_mother_and_child(&little_sensor_Disc);
		//~ world.set_mother_and_child(&Disc_little_sensor);
		//~ */
		//~ //world.set_mother_and_child(&sensor_Disc);
		//~ //world.set_mother_and_child(&cylinder);
		//~ //world.set_mother_and_child(&cylinder2);
		//~ 
		//~ //test_ellipsoid.disp();
		//~ 
		//~ //==============================================================
		//~ //post init
		//~ //==============================================================
		//~ world.post_initialize_me_and_all_my_children();
		//~ //post_init_world(&world);
//~ 
		//~ //==============================================================
		//~ // tracing
		//~ //==============================================================
		//~ stringstream export_file_name;
		//~ export_file_name.str("");
		//~ export_file_name<<"FACT";
		//~ export_file_name<<"_alpha"<<alpha_Davies_Cotton_parabola;
		//~ export_file_name<<"_ROT_mDEG";
		//~ export_file_name<<setprecision(4);
		//~ export_file_name<<"_X"<<dbl_FACT_X_rotangle_in_grad*1E3;
		//~ export_file_name<<"_Y"<<dbl_FACT_Y_rotangle_in_grad*1E3; 
		//~ export_file_name<<"_Z"<<dbl_FACT_Z_rotangle_in_grad*1E3<<"_";
		//~ //cout<<export_file_name.str();
//~ 
		//~ cout<<"//==============================================="<<endl;
		//~ cout<<"//	TRACING"<<endl;
		//~ cout<<"//==============================================="<<endl;
		//~ stringstream sstr_file_name_pin_hole_cam;
		//~ sstr_file_name_pin_hole_cam<<export_file_name.str()<<"pin_hole_cam";
		//~ 
		//~ Vector3D pos_cam;
		//~ Rotation3D dir_euler_cam;
		//~ PinHoleCamera cam;
		//~ pos_cam.set_vec3D(-10.0,0.0,3.5);
		//~ dir_euler_cam.set_rotation(0.0,-((M_PI*2.0)/360.0)*95.0,0.0);
		//~ cam.set_cam("pin_hole_cam",pos_cam,dir_euler_cam,640,640);
		//~ cam.set_pin_hole_cam(((M_PI*2.0)/360.0)*42);
		//~ //cam.disp();
		//~ //cam.cam_acquire_image(&world,&settings,sstr_file_name_pin_hole_cam.str());
		//~ //cam.cam_acquire_image_parallel(&world,&settings);
		//~ //cam.cam_save_image(sstr_file_name_pin_hole_cam.str());
		//~ 
		//~ /*
		//~ ApertureCamera Mamiya645;
		//~ pos.set_vec3D(-6.0,0.0,1.75);
		//~ rot.set_rotation(0.0,-((M_PI*2.0)/360.0)*105.0,0.0);
		//~ double Mamiya_F_stop_number = 2.3;
		//~ double Mamiya_focal_length_in_m = 0.08;
		//~ double Mamiya_sensor_width_in_m = 0.06;
		//~ double Mamiya_sensor_hight_in_m = 0.045;
		//~ double Mamiya_number_of_rays_emitted_per_pixel = 5;
		//~ Mamiya645.set_cam("Mamiya645",pos,rot,720,1);
		//~ Mamiya645.set_aperture_cam(
			//~ Mamiya_F_stop_number,
			//~ Mamiya_focal_length_in_m,
			//~ Mamiya_sensor_width_in_m,
			//~ Mamiya_sensor_hight_in_m,
			//~ Mamiya_number_of_rays_emitted_per_pixel);
			//~ 
		//~ Mamiya645.set_object_distance(4.0);
		//~ Mamiya645.disp();
		//~ string export_name_Mamiya645 = export_file_name.str() += "Mamiya645";
		//~ Mamiya645.cam_acquire_image(&world,&settings,export_name_Mamiya645);
		//~ */
		//~ 
		
		// free orbit
		//~ FreeOrbitCamera free;
		//~ free.set_free_orbit(Mworld,&settings);
		//~ free.start_free_orbit();
		
		//==================================================================
		/*
		double light_source_opening_angle_in_rad = (M_PI*2.0f)/360.0*0.012;
		
		LightSource bulb;
		pos.set_vec3D(0.0,0.0,10E3);
		rot.set_rotation(0.0,-M_PI,0.0);
		bulb.set_opening_angle(light_source_opening_angle_in_rad);
		bulb.set_cam("bulb",pos,rot,1E5,1);
		bulb.radiate(&world,&settings);
		
		// FoV of bulb
		
		cam.set_cam("bulb_cam",pos,rot,640,640);
		cam.set_pin_hole_cam(2.0*light_source_opening_angle_in_rad);
		cam.disp();

		string export_bulb_cam_name = export_file_name.str() += "bulb_cam";
		cam.cam_acquire_image_parallel(&world,&settings);
		cam.cam_save_image(export_bulb_cam_name);
		
		bulb.disp_bulb();
		sensor_Disc.disp();
		
		string export_csv_table_name = export_file_name.str() += "csv_table.csv";
		sensor_Disc.export_csv_table_of_intersections(export_csv_table_name);
		sensor_Disc.clear_intersection_list();
		
		string export_shadow_csv_table_name = export_file_name.str() += "shadow_csv_table.csv";
		garbage_esensor_Disc.
		export_csv_table_of_intersections(export_shadow_csv_table_name);
		garbage_esensor_Disc.clear_intersection_list();
		
		string export_telescope_table_name = export_file_name.str() += "telescope_table.txt";
		fact.export_table_of_telescope(export_telescope_table_name);
		*/
//}
	return 0;
}
