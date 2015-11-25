#include "ZdAzMount.h"

namespace ZdAzMount {
	//--------------------------------------------------------------------------
	Rotation3D get_rot_Az_Zd(const double az, const double zd) {
		return Rotation3D(0.0, zd, Deg2Rad(180.0) - az);
	}
	//--------------------------------------------------------------------------
	double get_zd_of_frame(const Frame* frame) {
		Vector3D optical_axis_in_world = frame->frame2world()->
			get_transformed_orientation(Vector3D::unit_z);

		return 
			Vector3D::unit_z.get_angle_in_between_in_rad(optical_axis_in_world);	
	}
	//--------------------------------------------------------------------------
	double get_az_of_frame(const Frame* frame) {
		return Deg2Rad(180.0) - frame->get_rotation_in_mother()->get_rot_z();
	}
	//--------------------------------------------------------------------------
} // AltitudeAzimuthMount
