#include "MmcsCorsikaSubBlock.h"
//------------------------------------------------------------------------------
MmcsCorsikaSubBlock::MmcsCorsikaSubBlock() {
	init_sub_block();
}
//------------------------------------------------------------------------------
std::string MmcsCorsikaSubBlock::get_4char_string_representation_at(const unsigned int i)const {
	assert_range_is_valid(i);
	return MmcsCorsikaTools::float2str_4byte_bin_map(sub_block[i]);
}
//------------------------------------------------------------------------------
bool MmcsCorsikaSubBlock::get_bool_representation_at(const unsigned int i)const {
	assert_range_is_valid(i);
	return  bool(sub_block[i]);
}
//------------------------------------------------------------------------------
void MmcsCorsikaSubBlock::assert_range_is_valid(const unsigned int i)const {
	if(i >= sub_block_size_in_words) {
		std::stringstream info;
		info << "MmcsCorsikaSubBlock:\n";
		info << "Invalid range. Expected 0 <= i < " << size();
		info << ", but actual i = " << i << "\n";
		throw std::runtime_error(info.str());
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
unsigned int MmcsCorsikaSubBlock::size_in_bytes()const {
	return sub_block_size_in_bytes;
}
//------------------------------------------------------------------------------
unsigned int MmcsCorsikaSubBlock::size()const {
	return size_in_words();
}
//------------------------------------------------------------------------------
unsigned int MmcsCorsikaSubBlock::size_in_words()const {
	return sub_block_size_in_words;
}
//------------------------------------------------------------------------------
void MmcsCorsikaSubBlock::init_sub_block() {
	sub_block.clear();
	sub_block.resize(sub_block_size_in_words, 0.0);
} 
//------------------------------------------------------------------------------
void MmcsCorsikaSubBlock::operator=(MmcsCorsikaSubBlock eq) {
	sub_block = eq.sub_block;
} 
//------------------------------------------------------------------------------
bool MmcsCorsikaSubBlock::is_event_header()const {
	return sub_block[0] == MmcsCorsikaTools::str2float_4byte_bin_map("EVTH");
}
//------------------------------------------------------------------------------
bool MmcsCorsikaSubBlock::is_event_end()const {
	return sub_block[0] == MmcsCorsikaTools::str2float_4byte_bin_map("EVTE");
}
//------------------------------------------------------------------------------
bool MmcsCorsikaSubBlock::is_run_footer()const {
	return sub_block[0] == MmcsCorsikaTools::str2float_4byte_bin_map("RUNE");
}
//------------------------------------------------------------------------------
bool MmcsCorsikaSubBlock::is_run_header()const {
	return sub_block[0] == MmcsCorsikaTools::str2float_4byte_bin_map("RUNH");
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
	std::cout << str();
}
//------------------------------------------------------------------------------
std::string MmcsCorsikaSubBlock::str()const {
	std::stringstream out;

	for(unsigned int row=0; row<size_in_words()/number_of_columns; row++)
		out << get_print_of_row_number(row) << "\n";

	return out.str();
}
//------------------------------------------------------------------------------
std::string MmcsCorsikaSubBlock::get_print_of_row_number(const unsigned int row)const {
	std::stringstream out;

	for(unsigned int col=0; col<number_of_columns; col++) {
		const unsigned int c = row*number_of_columns+col;
		assert_range_is_valid(c);
		out << sub_block.at(c) << " ";		
	}	
	return out.str();
}
//------------------------------------------------------------------------------
unsigned int MmcsCorsikaSubBlock::cols()const {
	return number_of_columns;
}
//------------------------------------------------------------------------------
unsigned int MmcsCorsikaSubBlock::rows()const {
	return size_in_words()/number_of_columns;
}
//------------------------------------------------------------------------------
std::vector<float> MmcsCorsikaSubBlock::get_words_from_until(
	const unsigned int begin, 
	const unsigned int end
)const {
	std::vector<float> words;
	for(unsigned int j=begin; j<end; j++) {
		assert_range_is_valid(j);
		words.push_back( sub_block[j] );
	}
	return words;		
}
//------------------------------------------------------------------------------
float MmcsCorsikaSubBlock::get_word(const unsigned int at)const {
	assert_range_is_valid(at);
	return sub_block[at];	
}