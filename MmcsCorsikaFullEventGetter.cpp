#include "MmcsCorsikaFullEventGetter.h"
//------------------------------------------------------------------------------
MmcsCorsikaFullEventGetter::MmcsCorsikaFullEventGetter(
	const std::string _filename) : 
	filename(_filename), 
	sub_block_getter(_filename) 
{
	sub_block = sub_block_getter.get_next_sub_block();	
	run_header.create_from_mmcs_sub_block(sub_block);	
	//std::cout << "is_run_header" << ToolBox::float_2_four_byte_str_binary_mapping(sub_block[0]) << std::endl;
}
//------------------------------------------------------------------------------
MmcsCorsikaEvent MmcsCorsikaFullEventGetter::get_next_event() {

	MmcsCorsikaSubBlock   event_header;
	MmcsCorsikaPhotonData photon_data;
	MmcsCorsikaSubBlock   event_end;

	bool event_is_not_complete_yet = true;
	//std::cout << "get_next_event" << std::endl;
	while(event_is_not_complete_yet && sub_block_getter.has_still_sub_blocks_left()) {

		sub_block = sub_block_getter.get_next_sub_block();	

		if(sub_block.is_event_header()) {
			assert_event_header_is_only_assigned_once(event_header);
			event_header = sub_block;
			//std::cout << "is_event_header" <<  ToolBox::float_2_four_byte_str_binary_mapping(sub_block[0]) << std::endl;
		}
		
		if(sub_block.is_photon_data()) {
			photon_data.append_valid_photons_from_sub_block(sub_block);
			//std::cout << "is_photon_data" <<  ToolBox::float_2_four_byte_str_binary_mapping(sub_block[0]) << std::endl;
		}

		if(sub_block.is_event_end()) {
			assert_event_end_is_only_assigned_once(event_end);
			event_end = sub_block;
			event_is_not_complete_yet = false;
			//std::cout << "is_event_end" <<  ToolBox::float_2_four_byte_str_binary_mapping(sub_block[0]) << std::endl;
		}

		if(sub_block.is_run_footer()) {
			event_is_not_complete_yet = false;
			//std::cout << "is_run_footer" <<  ToolBox::float_2_four_byte_str_binary_mapping(sub_block[0]) << std::endl;
		}
	}

	if(!has_still_events_left())
		assign_the_remaining_sub_block_to_the_run_footer();

	//photon_data.print();
	MmcsCorsikaEvent event(event_header,photon_data,event_end);
	return event;
}
//------------------------------------------------------------------------------
void MmcsCorsikaFullEventGetter::assign_the_remaining_sub_block_to_the_run_footer() {
		assert_is_run_footer(sub_block);
		run_footer = sub_block;
}
//------------------------------------------------------------------------------
void MmcsCorsikaFullEventGetter::assert_is_run_footer(MmcsCorsikaSubBlock &sub_block)const {
	if(!sub_block.is_run_footer()) {
		std::stringstream info;
		info << "MmcsCorsikaFullEventGetter:\n";
		info << "Cant find a run footer in MMCS file: '" << filename << "'\n";
		throw TracerException(info.str());	
	}
}
//------------------------------------------------------------------------------
void MmcsCorsikaFullEventGetter::assert_event_header_is_only_assigned_once(MmcsCorsikaSubBlock &event_header)const {
	if(event_header.is_event_header()) {
		std::stringstream info;
		info << "MmcsCorsikaFullEventGetter:\n";
		info << "Found at least 2 event header (EVTH) ";
		info << "before event end (EVTE) in MMCS file: '" << filename << "'\n";
		info << "There must only be one event header per event.\n";
		throw TracerException(info.str());	
	}
}
//------------------------------------------------------------------------------
void MmcsCorsikaFullEventGetter::assert_event_end_is_only_assigned_once(MmcsCorsikaSubBlock &event_end)const {
	if(event_end.is_event_header()) {
		std::stringstream info;
		info << "MmcsCorsikaFullEventGetter:\n";
		info << "Found at least 2 event ends (EVTE) ";
		info << "after event header (EVTH) in MMCS file: '" << filename << "'\n";
		info << "There must only be one event end per event.\n";
		throw TracerException(info.str());	
	}
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