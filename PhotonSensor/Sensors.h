//=================================
// include guard
#ifndef __PHOTONSENSORS_H_INCLUDED__
#define __PHOTONSENSORS_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "PhotonSensor/PhotonSensor.h"
using std::vector;
//=================================

namespace PhotonSensor {

	class Sensors {

	public:
		vector<PhotonSensor::Sensor*> by_occurence;
		vector<PhotonSensor::Sensor*> by_frame;
		vector<uint> occurence2frame_indices;
		
		Sensors();
		Sensors(vector<PhotonSensor::Sensor*> &sensors);
		void init(vector<PhotonSensor::Sensor*> &sensors);
		uint size()const;
		PhotonSensor::Sensor* at(const uint pos);
		PhotonSensor::Sensor* at_frame(const Frame* frame);
		void assign_photon(const Photon* photon);
		void assign_photons(const vector<Photon> *photons);
		void clear_history();
	private:

		void init_indices_occurence2frame();
		void init_sensors_by_frames();
		void assert_no_two_sensors_have_same_frame()const;
		uint occurence_index_given_frame_index(const uint frame_idx)const;
	public:

		class NoSuchFrame : public std::out_of_range {
			using out_of_range::out_of_range;
		};

		class DuplicateFrame : public std::invalid_argument {
			using invalid_argument::invalid_argument;
		};
	};
} // PhotonSensor
//------------------------------------------------------------------------------
#endif // __PHOTONSENSORS_H_INCLUDED__ 