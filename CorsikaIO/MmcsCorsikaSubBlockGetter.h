//=================================
// include guard
#ifndef __MMCSCORSIKASUBBLOCKGETTER_H_INCLUDE__
#define __MMCSCORSIKASUBBLOCKGETTER_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream> 
#include <string>
#include "Core/TracerException.h"
#include "Tools/Tools.h"
#include "Tools/FileTools.h"
#include "Tools/StringTools.h"
#include <fstream>
#include <stdio.h>
#include <string.h>
#include "MmcsCorsikaSubBlock.h"

//=================================
class MmcsCorsikaSubBlockGetter {
    const uint block_size_in_bytes = 22932;
    const uint block_size_in_words = block_size_in_bytes / 4;

    float *block;

    MmcsCorsikaSubBlock SubBlock;

    uint sub_block_in_block_counter = 0;
    uint block_counter = 0;

	const std::string filename;
    std::ifstream MMCS_file;
    uint file_size_in_bytes;
public:
    MmcsCorsikaSubBlockGetter(const std::string filename);
    ~MmcsCorsikaSubBlockGetter();
    bool has_still_sub_blocks_left()const;
    MmcsCorsikaSubBlock get_next_sub_block();
	void print();
	std::string get_print();
	void print_sub_block()const;
private:
   	bool is_a_gzip_file()const;
	void handle_gzip_file()const;
	void open_MMCS_file();
	void assert_MMCS_file_is_valid()const;
	void assert_file_reading_is_fine(const std::ifstream &file)const;
	void assert_file_size_is_multiple_of_MMCS_block_size()const;
	void remember_MMCS_file_size_in_bytes();
	uint current_position_in_MMCS_file_in_bytes();
	bool enough_MMCS_file_left_to_read_next_block();
	char* read_binary_block_from_MMCS_file();
	float* read_block_from_MMCS_file();
	void init_sub_block_array();
	void copy_sub_block_from_block();
	void read_next_block();
	void read_next_sub_block_from_block();
	bool block_has_already_been_used()const;
	void delete_old_data_in_block();
	uint start_for_sub_block_in_block_in_words()const;
	uint end_for_sub_block_in_block_in_words()const;
	uint file_size_left_in_bytes();
	uint number_of_sub_blocks_in_block()const;
	bool has_still_sub_blocks_left_on_block()const;
	uint total_number_of_sub_blocks()const;
	uint sum_of_sub_blocks_so_far()const;
	uint number_of_completed_blocks()const;
	void throw_can_not_open_Mmcs_file(const std::string details)const;
};
#endif // __MMCSCORSIKASUBBLOCKGETTER_H_INCLUDE__ 
