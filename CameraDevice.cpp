#include "CameraDevice.h"
//======================================================================
bool CameraDevice::set_cam(	
std::string new_camera_name, 
Vector3D 	new_camera_position_in_World,
Rotation3D 	new_camera_orientation_in_World, 
int 		newSensorResolutionU,
int 		newSensorResolutionV){
	//======================
	// camera CameraName
	//======================
	CameraName = new_camera_name;
	
	//======================
	//init camera position
	//======================
	CameraPositionInWorld = new_camera_position_in_World;
	
	//======================
	//init camera orientation
	//======================
	CameraOrientationInWorld = new_camera_orientation_in_World;
	//======================
	// calculate homgene Transformation
	//======================
	T_Camera2World.set_transformation(
	new_camera_orientation_in_World,
	new_camera_position_in_World
	);
	
	//======================
	// calculate direction of optical axis using T_Camera2World
	// default camera direction is unit-z vector.
	//======================
	CameraPointingDirection.set_unit_vector_z();
	T_Camera2World.transform_orientation(&CameraPointingDirection);
	//CameraPointingDirection.disp();
	//======================
	// calculate optical axis
	//======================
	OpticalAxis.SetRay(CameraPositionInWorld,CameraPointingDirection);
	
	//======================
	//init sensor properties
	//======================
	if(newSensorResolutionU > 0.0){
		
		if(newSensorResolutionU%2 != 0)
		{
			newSensorResolutionU++; 
			std::cout<<"CameraDevice->set_cam(): ";
			std::cout<<"Resolution u: "<<newSensorResolutionU<<std::endl;
		}
		SensorResolutionU = newSensorResolutionU;
		
	}else{
		std::stringstream out; out.str("");
		out<<"Sensor resolution in u can not be negative!";
		out<<" newSensorResolutionU = ";
		out<<newSensorResolutionU<<std::endl;
		std::cout<<out.str();
		return false;
	}
	if(newSensorResolutionV > 0.0){
		
		if(newSensorResolutionV%2 != 0)
		{
			newSensorResolutionV++; 
			std::cout<<"CameraDevice->set_cam(): ";
			std::cout<<"Resolution v: "<<newSensorResolutionV<<std::endl;
		}
		SensorResolutionV = newSensorResolutionV;
		
	}else{
		std::stringstream out; out.str("");
		out<<"Sensor resolution in v can not be negative!";
		out<<" newSensorResolutionV = ";
		out<<newSensorResolutionU<<std::endl;
		std::cout<<out.str();
		return false;
	}
	SensorResolutionUTimesV = SensorResolutionU*SensorResolutionV;
	return true;
}
//======================================================================
void CameraDevice::set_pointing_direction(	
Vector3D new_camera_pointing_direction_in_World,
Vector3D new_camera_image_upwards_image_orientation_in_world){
	
	new_camera_pointing_direction_in_World = 
	new_camera_pointing_direction_in_World/
	new_camera_pointing_direction_in_World.norm2();
	
	// image orientation is defined by x,y.
	// camera y is supposed to point "upwards" in world system
	
	Vector3D rotated_z = new_camera_pointing_direction_in_World;
	Vector3D rotated_x;
	Vector3D rotated_y = 
	new_camera_image_upwards_image_orientation_in_world;
	
	rotated_x = rotated_y.CrossProduct(rotated_z);
	
	//~ std::cout<<"CameraDevice -> set_pointing_direction() -> ";
	//~ std::cout<<"x: "<<rotated_x<<",y: "<<rotated_y<<",z: "<<rotated_z;
	//~ std::cout<<std::endl;
	//======================
	//init / reinit camera orientation
	//======================
	T_Camera2World.set_transformation(
	rotated_x,
	rotated_y,
	rotated_z,
	CameraPositionInWorld
	);
	
	//======================
	// calculate direction of optical axis using T_Camera2World
	// default camera direction is unit-z vector.
	//======================
	CameraPointingDirection.set_unit_vector_z();
	T_Camera2World.transform_orientation(&CameraPointingDirection);
	//CameraPointingDirection.disp();
	//======================
	// calculate optical axis
	//======================
	OpticalAxis.SetRay(CameraPositionInWorld,CameraPointingDirection);
}
//======================================================================
Vector3D CameraDevice::get_image_upwards_direction_in_world_frame()const{
	// image upwards direction in camera frame
	Vector3D image_upwards_direction_in_world_frame;
	image_upwards_direction_in_world_frame.set_unit_vector_y();
	
	// image upwards direction in world frame
	T_Camera2World.transform_orientation(
	&image_upwards_direction_in_world_frame);
	
	return image_upwards_direction_in_world_frame;
}
//======================================================================
std::string CameraDevice::get_cam_string()const{
	std::stringstream out; out.str("");
	out<<std::endl;
	out<<"camera: "<<CameraName<<"________________"<<std::endl;
	out<<"|T_"<<CameraName<<"2world:"<<std::endl;
	out<<T_Camera2World.get_string();	
	out<<"|camera position          : ";
	out<<CameraPositionInWorld.get_string()<<std::endl;
	out<<"|direction of optical axis: ";
	out<<CameraPointingDirection.get_string()<<std::endl;
	return out.str();
}
//======================================================================
Vector3D CameraDevice::get_pointing_direction()const{
	return CameraPointingDirection;
}
//======================================================================
void CameraDevice::disp()const{
	std::cout<<get_cam_string();
}
//======================================================================
void CameraDevice::calculate_stereo_parameters(
const CartesianFrame* world,
const GlobalSettings* settings,
const double cmaera_offset_in_m,
Vector3D &left_camera_position,
Vector3D &left_camera_pointing_direction,	
Vector3D &right_camera_position,
Vector3D &right_camera_pointing_direction)const{
	//~ std::cout<<"CameraDevice-> stereo -> cmaera offset: ";
	//~ std::cout<<cmaera_offset_in_m<<"[m]"<<std::endl;
	
	double object_distance = OpticalAxis.
	get_distance_to_closest_object(world,0,NULL,settings,0.0);
	
	//~ std::cout<<"CameraDevice-> stereo -> camera position: ";
	//~ std::cout<<CameraPositionInWorld<<std::endl;
	
	// default
	if(object_distance==0.0)
		object_distance = 100.0;
	
	//~ std::cout<<"CameraDevice-> stereo -> object distance: ";
	//~ std::cout<<object_distance<<"[m]"<<std::endl;
	
	//calculate intersection point of center ray
	Vector3D intersection_point = 
	OpticalAxis.PositionOnRay(object_distance);
	
	//~ std::cout<<"CameraDevice-> stereo -> ";
	//~ std::cout<<"intersection point of optical axis and object: ";
	//~ std::cout<<intersection_point<<std::endl;	
	
	Vector3D z_unit; 
	
	z_unit.set_unit_vector_z();
	
	Vector3D axis_direction_of_stereo_cameras = 
	CameraPointingDirection.CrossProduct(z_unit);

	axis_direction_of_stereo_cameras = 
	axis_direction_of_stereo_cameras/
	axis_direction_of_stereo_cameras.norm2();
	
	//~ std::cout<<"CameraDevice-> stereo -> ";
	//~ std::cout<<"axis_direction_of_stereo_cameras: ";
	//~ std::cout<<axis_direction_of_stereo_cameras<<std::endl;		
	
	//left camera position
	left_camera_position = 
	CameraPositionInWorld + 
	axis_direction_of_stereo_cameras*cmaera_offset_in_m/2.0;
	
	//left camera pointing direction
	left_camera_pointing_direction = 
	intersection_point - left_camera_position;

	left_camera_pointing_direction = 
	left_camera_pointing_direction/
	left_camera_pointing_direction.norm2();
	
	//~ std::cout<<"CameraDevice-> stereo -> ";
	//~ std::cout<<"left  camera pos: ";
	//~ std::cout<<left_camera_position<<", ";			
	//~ std::cout<<"dir: ";
	//~ std::cout<<left_camera_pointing_direction<<std::endl;	
		
	//right camera position
	right_camera_position = 
	CameraPositionInWorld - 
	axis_direction_of_stereo_cameras*cmaera_offset_in_m/2.0;
		
	//right camera pointing direction
	right_camera_pointing_direction = 
	intersection_point - right_camera_position;
	
	right_camera_pointing_direction = 
	right_camera_pointing_direction/
	right_camera_pointing_direction.norm2();
	
	//~ std::cout<<"CameraDevice-> stereo -> ";
	//~ std::cout<<"right camera pos: ";
	//~ std::cout<<right_camera_position<<", ";		
	//~ std::cout<<"dir: ";
	//~ std::cout<<right_camera_pointing_direction<<std::endl;
	
}
//======================================================================
