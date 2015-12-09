#include "Cameras/SkyDome/Dome.h"
#include "math.h"

using namespace SkyDome;

double Dome::get_zenith_distance_of(const Vector3D dir)const {
	return Vector3D::unit_z.get_angle_in_between_in_rad(dir);
}

double Dome::get_azimuth_angle_of(const Vector3D dir)const {
	return atan2(dir.y(),dir.x());
}