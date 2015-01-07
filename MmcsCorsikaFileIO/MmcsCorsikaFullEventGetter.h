//=================================
// include guard
#ifndef __MMCSCORSIKAFULLEVENTGETTER_H_INCLUDE__
#define __MMCSCORSIKAFULLEVENTGETTER_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "MmcsCorsikaSubBlockGetter.h"
#include "MmcsCorsikaSubBlock.h"
#include "MmcsCorsikaRunHeader.h"
#include "MmcsCorsikaPhotonData.h"
#include "MmcsCorsikaEvent.h"
#include "FiniteStateMashine.h"
#include "Functions.h"

namespace MMCS {
	enum states_during_file_reading {
		waiting_for_first_sub_block,
		found_run_header_waiting_for_event_header,
		found_event_header_waiting_for_at_least_one_photon_data_block,
		found_at_least_one_photon_data_block_waiting_for_event_end,
		found_event_end_waiting_for_another_event_header_or_run_footer,
		found_run_footer,
		file_valid_and_complete	
	};

	enum events_during_file_reading {
		sub_block_is_run_header,
		sub_block_is_event_header,
		sub_block_is_photon_data,
		sub_block_is_event_end,
		sub_block_is_run_footer
	};
}
//=================================
class MmcsCorsikaFullEventGetter {

	const std::string filename;

	MmcsCorsikaSubBlockGetter sub_block_getter;
	MmcsCorsikaSubBlock sub_block;

	MmcsCorsikaRunHeader run_header;
	MmcsCorsikaSubBlock run_footer;

	FiniteStateMashine Mmcs_acceptor;

	MmcsCorsikaSubBlock next_event_header;
public:
	MmcsCorsikaFullEventGetter(const std::string _filename);
	MmcsCorsikaEvent get_next_event();
	bool has_still_events_left()const;
private:
	void get_next_sub_block();
	bool event_is_complet()const;
	void get_next_sub_block_from_mmcs_file();
	void assign_the_remaining_sub_block_to_the_run_footer();
	void get_photon_data_from_sub_block(MmcsCorsikaPhotonData &photon_data)const;
	bool latest_sub_block_is_run_footer()const;
	void assert_is_run_footer(MmcsCorsikaSubBlock &sub_block)const;
	void assert_event_header_is_only_assigned_once(MmcsCorsikaSubBlock &event_header)const;
	void assert_event_end_is_only_assigned_once(MmcsCorsikaSubBlock &event_end)const;
	void assert_MMCS_file_syntax_is_valid(const MmcsCorsikaSubBlock &sub_block);
	void initialize_MMCS_file_acceptor();
	void MMCS_acceptor_define_states();
	void MMCS_acceptor_define_transitions();
	int get_type_for_acceptor(const MmcsCorsikaSubBlock &sub_block)const;


};
#endif // __MMCSCORSIKAFULLEVENTGETTER_H_INCLUDE__ 
