#include "MmcsCorsikaSubBlock.h"
MmcsCorsikaSubBlock::MmcsCorsikaSubBlock() {
	init_sub_block();
}
//------------------------------------------------------------------------------
std::string MmcsCorsikaSubBlock::get_4char_string_representation_at(uint at)const {
	assert_range_is_valid(at);
	return ToolBox::float_2_four_byte_str_binary_mapping(sub_block[at]);
}
//------------------------------------------------------------------------------
bool MmcsCorsikaSubBlock::get_bool_representation_at(uint at)const {
	assert_range_is_valid(at);
	return  bool(sub_block[at]);
}
//------------------------------------------------------------------------------
void MmcsCorsikaSubBlock::assert_range_is_valid(uint at)const {
	if(at > sub_block_size_in_words) {
		std::stringstream info;
		info << "MmcsCorsikaSubBlock:\n";
		info << "Invalid range. Expected 0 <= at < " << size();
		info << ", but actual at = " << at << "\n";
		throw TracerException(info.str());
	}
}
//------------------------------------------------------------------------------
float& MmcsCorsikaSubBlock::operator[] (const size_t index) {
	assert_range_is_valid(index);
    return sub_block[index];
}
//------------------------------------------------------------------------------
const float& MmcsCorsikaSubBlock::operator[] (const size_t index) const {
	assert_range_is_valid(index);
    return sub_block[index];
}
//------------------------------------------------------------------------------
uint MmcsCorsikaSubBlock::size_in_bytes()const {
	return sub_block_size_in_bytes;
}
//------------------------------------------------------------------------------
uint MmcsCorsikaSubBlock::size()const {
	return size_in_words();
}
//------------------------------------------------------------------------------
uint MmcsCorsikaSubBlock::size_in_words()const {
	return sub_block_size_in_words;
}
//------------------------------------------------------------------------------
void MmcsCorsikaSubBlock::init_sub_block() {
	sub_block.clear();
	sub_block.resize(size(),0.0);
} 
//------------------------------------------------------------------------------
void MmcsCorsikaSubBlock::operator=(MmcsCorsikaSubBlock eq) {
	sub_block = eq.sub_block;
} 
//------------------------------------------------------------------------------
bool MmcsCorsikaSubBlock::is_event_header()const {
	return sub_block[0] == ToolBox::four_byte_str_2_float_binary_mapping("EVTH");
}
//------------------------------------------------------------------------------
bool MmcsCorsikaSubBlock::is_event_end()const {
	return sub_block[0] == ToolBox::four_byte_str_2_float_binary_mapping("EVTE");
}
//------------------------------------------------------------------------------
bool MmcsCorsikaSubBlock::is_run_footer()const {
	return sub_block[0] == ToolBox::four_byte_str_2_float_binary_mapping("RUNE");
}
//------------------------------------------------------------------------------
bool MmcsCorsikaSubBlock::is_run_header()const {
	return sub_block[0] == ToolBox::four_byte_str_2_float_binary_mapping("RUNH");
}
//------------------------------------------------------------------------------
bool MmcsCorsikaSubBlock::is_photon_data()const {
	if(is_event_header())
		return false;

	if(is_event_end())
		return false;

	if(is_run_header())
		return false;

	if(is_run_footer())
		return false;
	return true;
}
//------------------------------------------------------------------------------
void MmcsCorsikaSubBlock::print()const {
	std::cout << get_print();
}
//------------------------------------------------------------------------------
std::string MmcsCorsikaSubBlock::get_print()const {
	std::stringstream out;

	for(uint row=0; row<size_in_words()/number_of_columns; row++)
		out << get_print_of_row_number(row) << "\n";

	return out.str();
}
//------------------------------------------------------------------------------
std::string MmcsCorsikaSubBlock::get_print_of_row_number(const uint row)const {
	std::stringstream out;

	for(uint col=0; col<number_of_columns; col++) {
		const uint c = row*number_of_columns+col;
		assert_range_is_valid(c);
		out << sub_block.at(c) << " ";		
	}	
	return out.str();
}
//------------------------------------------------------------------------------
uint MmcsCorsikaSubBlock::cols()const {
	return number_of_columns;
}
//------------------------------------------------------------------------------
uint MmcsCorsikaSubBlock::rows()const {
	return size_in_words()/number_of_columns;
}