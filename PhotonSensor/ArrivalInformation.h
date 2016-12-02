//=================================
// include guard
#ifndef __PHOTONSENSOR_ARRIVALINFO_H_INCLUDED__
#define __PHOTONSENSOR_ARRIVALINFO_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies

//=================================
namespace PhotonSensor {

	struct ArrivalInformation {
		int simulation_truth_id;
		double wavelength;
		double arrival_time;
		double x_intersect;
		double y_intersect;
		double theta_x;
		double theta_y;
		
		ArrivalInformation();
		ArrivalInformation(
			int id,
			double w,
			double t,
			double x,
			double y,
			double tx,
			double ty
		);	
	};
} // PhotonSensor
#endif // __PHOTONSENSOR_ARRIVALINFO_H_INCLUDED__ 