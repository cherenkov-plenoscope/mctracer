//=================================
// include guard
#ifndef __MMCSCORSIKARUNHEADER_H_INCLUDE__
#define __MMCSCORSIKARUNHEADER_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <algorithm>
#include "MmcsCorsikaTools.h"
#include "MmcsCorsikaSubBlock.h"
#include "MmcsCorsikaSubBlockGetter.h"
//=================================
class MmcsCorsikaRunHeader {
    MmcsCorsikaSubBlock mmcs_sub_block;

	float run_number;
	int date_of_begin_run;
	float version_of_program;

	uint number_of_observation_levels;
	std::vector<float> observation_levels;

    float slope_of_energy_spektrum;
    std::vector<float> energy_range;

    bool flag_for_EGS4_treatment_of_em_component;
    bool flag_for_NKG_treatment_of_em_component;

    float kin_energy_cutoff_for_hadrons_in_GeV;
    float kin_energy_cutoff_for_muons_in_GeV;
	float kin_energy_cutoff_for_electrons_in_GeV;
	float energy_cutoff_for_photons_in_GeV;

	std::vector<float> phyiscal_constants;

	float X_displacement_of_inclined_observation_plane;
	float Y_displacement_of_inclined_observation_plane;
	float Z_displacement_of_inclined_observation_plane;

    float theta_angle_of_normal_vector_of_inclined_observation_plane;
    float phi_angle_of_normal_vector_of_inclined_observation_plane;
    
    std::vector<float> CKA;
    std::vector<float> CETA;
    std::vector<float> CSTRBA;
    float scatter_range_in_x_direction_for_Cherenkov;
    float scatter_range_in_y_direction_for_Cherenkov;
    std::vector<float> HLAY;
    std::vector<float> AATM;
    std::vector<float> BATM;
    std::vector<float> CATM;
    float NFLAIN;
    float NFLDIF;
    float NFLPI0_plus_100_times_NFLPIF;
    float NFLCHE_plus_100_times_NFRAGM;
public:
    MmcsCorsikaRunHeader();
    MmcsCorsikaRunHeader(const std::vector<float>& block);
    void create_from_mmcs_sub_block(const MmcsCorsikaSubBlock &mmcs_sub_block);    
   	void print()const;
    std::string get_print()const;
private:
	void fill_basic_run_information();
   	void fill_observation_levels();
	void assert_sub_block_has_Run_Header()const;
    void assert_number_of_observation_levels_is_valid()const;
	void fill_from_until(std::vector<float> &vec, const uint begin, const uint end);
	void fill_from(float &num_to_fill, const uint at);
};
#endif // __MMCSCORSIKARUNHEADER_H_INCLUDE__ 