#include "CameraManForPosition.h"

CameraManForPosition::CameraManForPosition(CameraDevice* camera_to_work_with) {
	camera = camera_to_work_with;
}

CameraManForPosition::set_default_position(){
	camera->SetPositionAndOrientation(default_position,default_orientation);
}