#include "MmcsCorsikaEventHeader.h"
//------------------------------------------------------------------------------
MmcsCorsikaEventHeader::MmcsCorsikaEventHeader(){}
//------------------------------------------------------------------------------
void MmcsCorsikaEventHeader::create_from_MMCS_sub_block(
	const MmcsCorsikaSubBlock &sub_block
) {
	assert_sub_block_is_event_header(sub_block);

    EVTH_word = sub_block.get_4char_string_representation_at(0);
    event_number = int(sub_block.get_word(1));
    particle_id = int(sub_block.get_word(2));
    total_energy_in_GeV = sub_block.get_word(3);
    starting_altitude_in_g_per_cm2 = sub_block.get_word(4);
    number_of_first_target_if_fixed = sub_block.get_word(5);
    z_coordinate_aka_height_of_first_interaction_in_cm = sub_block.get_word(6);

    px_momentum_in_x_direction_in_GeV_over_c = sub_block.get_word(7);
    py_momentum_in_y_direction_in_GeV_over_c = sub_block.get_word(8);
    pz_momentum_in_negative_z_direction_in_GeV_over_c = sub_block.get_word(9);

    zenith_angle_Theta_in_radian = sub_block.get_word(10);
    azimuth_angle_Phi_in_radian = sub_block.get_word(11);
    number_of_different_random_number_sequences_max_10 = 
    	int(sub_block.get_word(12));
    //integer_seed_of_sequence = sub_block.get_words_from_until();
    //std::vector<int> number_of_offset_random_calls_mod_10e6_of_sequence;
    //std::vector<int> number_of_offset_random_calls_div_10e6_of_sequence;
    run_number = int(sub_block.get_word(45));
    date_of_begin_run_yymmdd = int(sub_block.get_word(46));
    version_of_program = sub_block.get_word(47);
    number_of_observation_levels = int(sub_block.get_word(48));
    height_of_level_in_cm = 
    	sub_block.get_words_from_until(49,49+number_of_observation_levels);
    //----------------------------
    slope_of_energy_spectrum = sub_block.get_word(58);
    lower_limit_of_energy_range_in_GeV = sub_block.get_word(59);
    upper_limit_of_energy_range_in_GeV = sub_block.get_word(60);
    cutoff_for_hadrons_kinetic_energy_in_GeV = sub_block.get_word(61);
    cutoff_for_muons_kinetic_energy_in_GeV = sub_block.get_word(62);
	cutoff_for_electrons_kinetic_energy_in_GeV = sub_block.get_word(63);
	cutoff_for_photons_energy_in_GeV = sub_block.get_word(64);
	NFLAIN = sub_block.get_word(65);
	NFLDIF = sub_block.get_word(66);
	NFLPI0 = sub_block.get_word(67);
	NFLPIF = sub_block.get_word(68);
	NFLCHE = sub_block.get_word(69);
	NFRAGM = sub_block.get_word(70);
	x_component_of_Earth_s_magnetic_field_in_uT = sub_block.get_word(71);
	z_component_of_Earth_s_magnetic_field_in_uT = sub_block.get_word(72);
	flag_for_activating_EGS4 = sub_block.get_word(73);
	flag_for_activating_NKG = sub_block.get_word(74);
	low_energy_hadr_model_flag = sub_block.get_word(75);
	high_energy_hadr_model_flag = sub_block.get_word(76);
	CERENKOV_flag = sub_block.get_word(78);
	//----------------------------   
	NEUTRINO_flag = sub_block.get_word(79);
	CURVED_flag = sub_block.get_word(80);
	computer_flag = sub_block.get_word(81);
	//----------------------------
	lower_edge_of_Theta_interval_in_DEG = sub_block.get_word(82);
	upper_edge_of_Theta_interval_in_DEG = sub_block.get_word(83);
	lower_edge_of_Phi_interval_in_DEG = sub_block.get_word(84);
	upper_edge_of_Phi_interval_in_DEG = sub_block.get_word(85);
	//----------------------------
	Cherenkov_bunch_size_in_the_case_of_Cherenkov_calculations = 
		sub_block.get_word(84);
	number_of_Cherenkov_detectors_in_x_direction = sub_block.get_word(85);
	number_of_Cherenkov_detectors_in_y_direction = sub_block.get_word(86);
	grid_spacing_of_Cherenkov_detectors_in_x_direction_in_cm = 
		sub_block.get_word(87);
	grid_spacing_of_Cherenkov_detectors_in_y_direction_in_cm = 
		sub_block.get_word(88);
	length_of_each_Cherenkov_detector_in_x_direction_in_cm = 
		sub_block.get_word(89);
	length_of_each_Cherenkov_detector_in_y_direction_in_cm = 
		sub_block.get_word(90);
	Cherenkov_output_directed_to_particle_output_file = sub_block.get_word(91);
	angle_between_array_x_direction_and_magnetic_north_in_rad = 
		sub_block.get_word(92);
	flag_for_additional_muon_information_on_particle_output_file = 
		sub_block.get_word(93);
	step_length_factor_for_multiple_scattering_step_length_in_EGS4 = 
		sub_block.get_word(94);
	Cherenkov_wavelength_lower_end_in_nm = sub_block.get_word(95);
	Cherenkov_wavelength_upper_end_in_nm = sub_block.get_word(96);
	number_of_uses_of_each_Cherenkov_event = sub_block.get_word(97);
    x_coordinate_of_core_location_for_scattered_events_in_cm = 
    	sub_block.get_words_from_until(
    		98,
    		98+number_of_uses_of_each_Cherenkov_event
    	);
    y_coordinate_of_core_location_for_scattered_events_in_cm = 
    	sub_block.get_words_from_until(
    		118,
    		118+number_of_uses_of_each_Cherenkov_event
    	);
}
//------------------------------------------------------------------------------
MmcsCorsikaEventHeader::MmcsCorsikaEventHeader(
	const MmcsCorsikaSubBlock &sub_block
) {
	create_from_MMCS_sub_block(sub_block);
}

MmcsCorsikaEventHeader::MmcsCorsikaEventHeader(
	const std::vector<float>& block
	)
{
	MmcsCorsikaSubBlock dummy;
	for (size_t i=0; i<block.size(); i++)
		dummy[i] = block[i];

	create_from_MMCS_sub_block(dummy);
}


//------------------------------------------------------------------------------
void MmcsCorsikaEventHeader::assert_sub_block_is_event_header(
	const MmcsCorsikaSubBlock &sub_block
) {
	if(!sub_block.is_event_header()) {
		std::stringstream info;
		info << "An event header must be parsed from a ";
		info << "MAGIC_Monte_Carlo_Simulation CORSIKA event header SubBlock.";
		info << "Expected the SubBlock to start with the event header Word: ";
		info << "'EVTH', but actually: '";
		info << sub_block.get_4char_string_representation_at(0) << "'.";
		throw TracerException(info.str());			
	}
}
//------------------------------------------------------------------------------
std::string MmcsCorsikaEventHeader::get_print()const {
	std::stringstream out;
	out << "Event_Header\n";
	out << "core positions: \n";
	out << " x.................... " << MmcsCorsikaTools::float_vec_2_str_using_delimiter(
		x_coordinate_of_core_location_for_scattered_events_in_cm, " ") << "cm\n";
	out << " y.................... " << MmcsCorsikaTools::float_vec_2_str_using_delimiter(
		y_coordinate_of_core_location_for_scattered_events_in_cm, " ") << "cm\n";
	out << "Telescope pointing\n";
	out << " Az................... " << Rad2Deg(azimuth_angle_Phi_in_radian) << "deg\n";
	out << " Zd................... " << Rad2Deg(zenith_angle_Theta_in_radian) << "deg\n";
	out << "Primary particle\n";
	out << " Corsika ID........... " << particle_id << "\n";
	out << " E.................... " << total_energy_in_GeV << "GeV\n";
	out << " start altitude....... " << starting_altitude_in_g_per_cm2 << "g/cm^2\n";
	out << " first interaction z.. " << z_coordinate_aka_height_of_first_interaction_in_cm*1e-2 << "m\n";
	out << " n. obs. levels....... " << number_of_observation_levels << "\n";
	//out << " obs. height a.s.l.... " << height_of_level_in_cm.at(0)*1e-2 << "m\n";
	return out.str();
}
