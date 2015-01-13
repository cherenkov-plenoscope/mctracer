//=================================
// include guard
#ifndef __MMCSCORSIKASUBBLOCK_H_INCLUDE__
#define __MMCSCORSIKASUBBLOCK_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <vector>
#include "MmcsCorsikaTools.h"
#include "Tools/StringTools.h"
#include "TracerException.h"
//=================================
class MmcsCorsikaSubBlock {

    const uint sub_block_size_in_bytes = 1092;
    const uint sub_block_size_in_words = sub_block_size_in_bytes / 4;
	const uint number_of_columns = 7;

	std::vector<float> sub_block;
public:
	MmcsCorsikaSubBlock();
    float& operator[] (const size_t);
    const float &operator[] (const size_t) const;
	std::string get_4char_string_representation_at(const uint i)const;
	uint size_in_bytes()const;
	uint size_in_words()const;
	uint size()const;
	void operator=(MmcsCorsikaSubBlock eq);
	bool is_event_header()const;
	bool is_event_end()const;
	bool is_run_footer()const;
	bool is_run_header()const;
	bool is_photon_data()const;
	bool get_bool_representation_at(const uint i)const;
	void print()const;
	std::string get_print()const;
	uint cols()const;
	uint rows()const;
	std::vector<float> get_words_from_until(const uint begin, const uint end)const;
	float get_word(const uint at)const;
private:
	void assert_range_is_valid(const uint i)const;
	void init_sub_block();
	std::string get_print_of_row_number(const uint row)const;
};
#endif // __MMCSCORSIKASUBBLOCK_H_INCLUDE__ 
