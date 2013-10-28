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
	OpticalAxis.set_ray(CameraPositionInWorld,CameraPointingDirection);
	
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
