#include "MmcsCorsikaFullEventGetter.h"
//------------------------------------------------------------------------------
MmcsCorsikaFullEventGetter::MmcsCorsikaFullEventGetter(
	const std::string _filename) : 
	filename(_filename), 
	sub_block_getter(_filename) 
{
	initialize_MMCS_file_acceptor();

	get_next_sub_block();
	run_header.create_from_mmcs_sub_block(sub_block);

	get_next_sub_block();
	next_event_header = sub_block;
}
//------------------------------------------------------------------------------
void MmcsCorsikaFullEventGetter::get_next_sub_block() {
	sub_block = sub_block_getter.get_next_sub_block();
	assert_MMCS_file_syntax_is_valid(sub_block);
}
//------------------------------------------------------------------------------
void MmcsCorsikaFullEventGetter::assert_MMCS_file_syntax_is_valid(
	const MmcsCorsikaSubBlock &sub_block
) {
	if(!Mmcs_acceptor.transition_given_event(get_type_for_acceptor(sub_block))){
		std::stringstream info;
		info << "MmcsCorsikaFullEventGetter:\n";
		info << "Bad MMCS file syntax in file: '" << filename << "'\n";
		info << "Mmcs_acceptor state: " << Mmcs_acceptor.get_state() << "\n";
		info << "Expected current sub_block to be: ?, but actual: ";
		info << sub_block.get_4char_string_representation_at(0) << " \n";
		throw std::runtime_error(info.str());			
	}
}
//------------------------------------------------------------------------------
MmcsCorsikaEvent MmcsCorsikaFullEventGetter::get_next_event() {

	MmcsCorsikaEventHeader event_header(next_event_header);// = next_event_header;
	MmcsCorsikaPhotonData photon_data;
	MmcsCorsikaSubBlock event_end;

	while(sub_block_getter.has_still_sub_blocks_left() && !event_is_complet()) {
		get_next_sub_block();
		
		if(sub_block.is_photon_data())
			photon_data.append_valid_photons_from_sub_block(sub_block);

		if(sub_block.is_event_end())
			event_end = sub_block;
	}

	if(sub_block_getter.has_still_sub_blocks_left()) {
		get_next_sub_block();
		
		if(sub_block.is_event_header())
			next_event_header = sub_block;

		if(sub_block.is_run_footer())
			run_footer = sub_block;
	}

	MmcsCorsikaEvent event(event_header,photon_data,event_end);
	return event;
}
//------------------------------------------------------------------------------
bool MmcsCorsikaFullEventGetter::event_is_complet()const {
	return 
		Mmcs_acceptor.get_state() == 
		MMCS::found_event_end_waiting_for_another_event_header_or_run_footer;
}
//------------------------------------------------------------------------------
bool MmcsCorsikaFullEventGetter::has_still_events_left()const {
	return !latest_sub_block_is_run_footer();
}
//------------------------------------------------------------------------------
bool MmcsCorsikaFullEventGetter::latest_sub_block_is_run_footer()const {
	return sub_block.is_run_footer();	
}
//------------------------------------------------------------------------------
void MmcsCorsikaFullEventGetter::initialize_MMCS_file_acceptor() {
	MMCS_acceptor_define_states();
	MMCS_acceptor_define_transitions();
	Mmcs_acceptor.initialize_in_state(MMCS::waiting_for_first_sub_block);
}
//------------------------------------------------------------------------------
void MmcsCorsikaFullEventGetter::MMCS_acceptor_define_states() {
	Mmcs_acceptor.define_state(
		MMCS::waiting_for_first_sub_block);

	Mmcs_acceptor.define_state(
		MMCS::found_run_header_waiting_for_event_header);

	Mmcs_acceptor.define_state(
		MMCS::found_event_header_waiting_for_at_least_one_photon_data_block);

	Mmcs_acceptor.define_state(
		MMCS::found_at_least_one_photon_data_block_waiting_for_event_end);

	Mmcs_acceptor.define_state(
		MMCS::found_event_end_waiting_for_another_event_header_or_run_footer);

	Mmcs_acceptor.define_state(
		MMCS::found_run_footer);	

	Mmcs_acceptor.define_state(
		MMCS::file_valid_and_complete);	
}
//------------------------------------------------------------------------------
void MmcsCorsikaFullEventGetter::MMCS_acceptor_define_transitions() {
//                                                                            //
// MMCS CORSIKA file reading state transition diagram:                        //
//                                                                            //
//                       _______EVTH________                                  //
//                      /                   \                                 //
//                      |                   |                                 //
//     RUNH      EVTH  \|/  PHOT      EVTE  |   RUNE                          //
// (0)------>(1)------>(2)------>(3)_----->(4)------>(5)                      //
// START                         /|\                FINAL                     //
//                              /   \                                         //
//                             |     |                                        //
//                              \___/                                         //
//                               PHOT                                         //
	// 0 --> 1
	Mmcs_acceptor.define_state_to_state_when_event(
		MMCS::waiting_for_first_sub_block, 
		MMCS::found_run_header_waiting_for_event_header,
		MMCS::sub_block_is_run_header
	);

	// 1 --> 2
	Mmcs_acceptor.define_state_to_state_when_event(
		MMCS::found_run_header_waiting_for_event_header, 
		MMCS::found_event_header_waiting_for_at_least_one_photon_data_block,
		MMCS::sub_block_is_event_header
	);

	// 2 --> 3
	Mmcs_acceptor.define_state_to_state_when_event(
		MMCS::found_event_header_waiting_for_at_least_one_photon_data_block, 
		MMCS::found_at_least_one_photon_data_block_waiting_for_event_end,
		MMCS::sub_block_is_photon_data
	);

	// 3 --> 3
	Mmcs_acceptor.define_state_to_state_when_event(
		MMCS::found_at_least_one_photon_data_block_waiting_for_event_end, 
		MMCS::found_at_least_one_photon_data_block_waiting_for_event_end,
		MMCS::sub_block_is_photon_data
	);

	// 3 --> 4
	Mmcs_acceptor.define_state_to_state_when_event(
		MMCS::found_at_least_one_photon_data_block_waiting_for_event_end, 
		MMCS::found_event_end_waiting_for_another_event_header_or_run_footer,
		MMCS::sub_block_is_event_end
	);

	// 4 --> 2
	Mmcs_acceptor.define_state_to_state_when_event(
		MMCS::found_event_end_waiting_for_another_event_header_or_run_footer, 
		MMCS::found_event_header_waiting_for_at_least_one_photon_data_block,
		MMCS::sub_block_is_event_header
	);

	// 4 --> 5
	Mmcs_acceptor.define_state_to_state_when_event(
		MMCS::found_event_end_waiting_for_another_event_header_or_run_footer, 
		MMCS::file_valid_and_complete,
		MMCS::sub_block_is_run_footer
	);
}
//------------------------------------------------------------------------------
int MmcsCorsikaFullEventGetter::get_type_for_acceptor(
	const MmcsCorsikaSubBlock &sub_block
)const {

	if(sub_block.is_event_header())
		return MMCS::sub_block_is_event_header;

	if(sub_block.is_event_end())
		return MMCS::sub_block_is_event_end;

	if(sub_block.is_run_header())
		return MMCS::sub_block_is_run_header;

	if(sub_block.is_run_footer())
		return MMCS::sub_block_is_run_footer;

	return MMCS::sub_block_is_photon_data;
}