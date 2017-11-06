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
//=================================
class MmcsCorsikaSubBlock {

    const unsigned int sub_block_size_in_bytes = 1092;
    const unsigned int sub_block_size_in_words = sub_block_size_in_bytes / 4;
	const unsigned int number_of_columns = 7;

	std::vector<float> sub_block;
public:
	MmcsCorsikaSubBlock();
    float& operator[] (const size_t);
    const float &operator[] (const size_t) const;
	std::string get_4char_string_representation_at(const unsigned int i)const;
	unsigned int size_in_bytes()const;
	unsigned int size_in_words()const;
	unsigned int size()const;
	void operator=(MmcsCorsikaSubBlock eq);
	bool is_event_header()const;
	bool is_event_end()const;
	bool is_run_footer()const;
	bool is_run_header()const;
	bool is_photon_data()const;
	bool get_bool_representation_at(const unsigned int i)const;
	void print()const;
	std::string str()const;
	unsigned int cols()const;
	unsigned int rows()const;
	std::vector<float> get_words_from_until(const unsigned int begin, const unsigned int end)const;
	float get_word(const unsigned int at)const;
private:
	void assert_range_is_valid(const unsigned int i)const;
	void init_sub_block();
	std::string get_print_of_row_number(const unsigned int row)const;
};
#endif // __MMCSCORSIKASUBBLOCK_H_INCLUDE__ 
