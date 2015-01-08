//=================================
// include_guard
#ifndef __MMCSCORSIKAEVENTHEADER_H_INCLUDE__
#define __MMCSCORSIKAEVENTHEADER_H_INCLUDE__

//=================================
// forward_declared_dependencies

//=================================
// included_dependencies
#include "MmcsCorsikaSubBlock.h"
#include "MmcsCorsikaSubBlockGetter.h"
#include "Functions.h"
//=================================
class MmcsCorsikaEventHeader {

    std::string EVTH_word;
    int event_number;
    int particle_id;
    float total_energy_in_GeV;
    float starting_altitude_in_g_per_cm2;
    float number_of_first_target_if_fixed;
    float z_coordinate_aka_height_of_first_interaction_in_cm;
        //(negative, if_tracking_starts_at_margin_of_atmosphere, see_TSTART)
    float px_momentum_in_x_direction_in_GeV_over_c;
    float py_momentum_in_y_direction_in_GeV_over_c;
    float pz_momentum_in_negative_z_direction_in_GeV_over_c;
        //(pz_is_positive_for_downward_going_particles)
    float zenith_angle_Theta_in_radian;
    float azimuth_angle_Phi_in_radian;
    //----------------------------
    int number_of_different_random_number_sequences_max_10;
    std::vector<int> integer_seed_of_sequence;
    std::vector<int> number_of_offset_random_calls_mod_10e6_of_sequence;
    std::vector<int> number_of_offset_random_calls_div_10e6_of_sequence;
    //----------------------------
    int run_number;
    int date_of_begin_run_yymmdd;
    float version_of_program;
    //----------------------------
    int number_of_observation_levels;
    std::vector<float> height_of_level_in_cm;
    //----------------------------
    float slope_of_energy_spectrum;
    float lower_limit_of_energy_range_in_GeV;
    float upper_limit_of_energy_range_in_GeV;
    float cutoff_for_hadrons_kinetic_energy_in_GeV;
    float cutoff_for_muons_kinetic_energy_in_GeV;
    float cutoff_for_electrons_kinetic_energy_in_GeV;
    float cutoff_for_photons_energy_in_GeV;
    float NFLAIN;
    float NFLDIF;
    float NFLPI0;
    float NFLPIF;
    float NFLCHE;
    float NFRAGM;
    float x_component_of_Earth_s_magnetic_field_in_uT;
    float z_component_of_Earth_s_magnetic_field_in_uT;
    float flag_for_activating_EGS4;
    float flag_for_activating_NKG;
    float low_energy_hadr_model_flag; //(1.=GHEISHA, 2.=UrQMD, 3.=FLUKA)
    float high_energy_hadr_model_flag; 
        // (0.=HDPM,1.=VENUS, 2.=SIBYLL, 3.=QGSJET, 4.=DPMJET, 5.= NE_X_US , 6.=EPOS)
    float CERENKOV_flag;
    //----------------------------   
    float NEUTRINO_flag;
    float CURVED_flag; //(0=standard, 2=CURVED)
    float computer_flag; //(3=UNIX, 4=Macintosh)
    //----------------------------
    float lower_edge_of_Theta_interval_in_DEG;
    float upper_edge_of_Theta_interval_in_DEG;
    float lower_edge_of_Phi_interval_in_DEG;
    float upper_edge_of_Phi_interval_in_DEG;
    //----------------------------
    float Cherenkov_bunch_size_in_the_case_of_Cherenkov_calculations;
    float number_of_Cherenkov_detectors_in_x_direction;
    float number_of_Cherenkov_detectors_in_y_direction;
    float grid_spacing_of_Cherenkov_detectors_in_x_direction_in_cm;
    float grid_spacing_of_Cherenkov_detectors_in_y_direction_in_cm;
    float length_of_each_Cherenkov_detector_in_x_direction_in_cm;
    float length_of_each_Cherenkov_detector_in_y_direction_in_cm;
    float Cherenkov_output_directed_to_particle_output_file;
        //(= 0.) or Cherenkov output file (= 1.)
    //----------------------------   
    float angle_between_array_x_direction_and_magnetic_north_in_rad;
    float flag_for_additional_muon_information_on_particle_output_file;
    float step_length_factor_for_multiple_scattering_step_length_in_EGS4;
    float Cherenkov_wavelength_lower_end_in_nm;
    float Cherenkov_wavelength_upper_end_in_nm;
    float number_of_uses_of_each_Cherenkov_event;
    std::vector<float> x_coordinate_of_core_location_for_scattered_events_in_cm;
    std::vector<float> y_coordinate_of_core_location_for_scattered_events_in_cm;
    //----------------------------
public:
    MmcsCorsikaEventHeader();    
    MmcsCorsikaEventHeader(const MmcsCorsikaSubBlock &sub_block);
    void create_from_MMCS_sub_block(const MmcsCorsikaSubBlock &sub_block); 
   	void print()const;
    std::string get_print()const;
private:
    void assert_sub_block_is_event_header(const MmcsCorsikaSubBlock &sub_block);
};
#endif // __MMCSCORSIKAEVENTHEADER_H_INCLUDE__ 