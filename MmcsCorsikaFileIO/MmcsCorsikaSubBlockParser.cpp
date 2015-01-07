#include "MmcsCorsikaSubBlockParser.h"
//------------------------------------------------------------------------------
void MmcsCorsikaSubBlockParser::create_from_mmcs_sub_block(
	const MmcsCorsikaSubBlock &mmcs_sub_block
) {
	this->mmcs_sub_block = mmcs_sub_block;
}
//------------------------------------------------------------------------------
void MmcsCorsikaSubBlockParser::assert_sub_block_is(
	const std::string expected_identifier
)const {

	const std::string actual_identifier = 
		ToolBox::float2str_4byte_bin_map(mmcs_sub_block[0]);

	if(!StringUtilities::is_equal(expected_identifier, actual_identifier)) {
		std::stringstream info;
		info << "MmcsCorsikaSubBlockParser:\n";
		info << "Expected first word of block to be: '" << expected_identifier;
		info << "', but actual: '" << actual_identifier << "'\n";
		throw TracerException(info.str());		
	}
}
//------------------------------------------------------------------------------
void MmcsCorsikaSubBlockParser::print()const {
	std::cout << get_print();
}
//------------------------------------------------------------------------------
std::string MmcsCorsikaSubBlockParser::get_print()const {
	std::stringstream out;
	out << "MMCS_Run_Header_________________________________________________\n";
	out << "|_______________________________________________________________\n";
 	return out.str();	
}
//------------------------------------------------------------------------------
void MmcsCorsikaSubBlockParser::fill_from_until(
	std::vector<float> &vec, const uint begin, const uint end
) {
	for(uint j=begin; j<end; j++)
		vec.push_back( mmcs_sub_block[j] );		
}
//------------------------------------------------------------------------------
float MmcsCorsikaSubBlockParser::get_word(const uint at) {
	num_to_fill = mmcs_sub_block[at];	
}