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
#include "Functions.h"
//=================================
class MmcsCorsikaFullEventGetter {

	const std::string filename;

	MmcsCorsikaSubBlockGetter sub_block_getter;
	MmcsCorsikaSubBlock sub_block;

	MmcsCorsikaRunHeader run_header;
	MmcsCorsikaSubBlock run_footer;

public:
	MmcsCorsikaFullEventGetter(const std::string _filename);
	MmcsCorsikaEvent get_next_event();
	bool has_still_events_left()const;
private:
	void get_next_sub_block_from_mmcs_file();
	void assign_the_remaining_sub_block_to_the_run_footer();
	void get_photon_data_from_sub_block(MmcsCorsikaPhotonData &photon_data)const;
	bool latest_sub_block_is_run_footer()const;
	void assert_is_run_footer(MmcsCorsikaSubBlock &sub_block)const;
	void assert_event_header_is_only_assigned_once(MmcsCorsikaSubBlock &event_header)const;
	void assert_event_end_is_only_assigned_once(MmcsCorsikaSubBlock &event_end)const;

};
#endif // __MMCSCORSIKAFULLEVENTGETTER_H_INCLUDE__ 
