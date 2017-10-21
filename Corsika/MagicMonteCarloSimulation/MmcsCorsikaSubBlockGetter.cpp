/*
	This is a C++ version of the python MMCS reader by Dominik Neise.
*/
#include "MmcsCorsikaSubBlockGetter.h"
//------------------------------------------------------------------------------
MmcsCorsikaSubBlockGetter::MmcsCorsikaSubBlockGetter(const std::string _filename)
: filename(_filename)
{
	if(is_a_gzip_file()) 
		handle_gzip_file();

	remember_MMCS_file_size_in_bytes();
	open_MMCS_file();
	assert_MMCS_file_is_valid();

	read_next_block();
}
//------------------------------------------------------------------------------
bool MmcsCorsikaSubBlockGetter::is_a_gzip_file()const { 
	return StringTools::is_ending(filename, ".gz");
}
//------------------------------------------------------------------------------
void MmcsCorsikaSubBlockGetter::handle_gzip_file()const {
	std::stringstream info;
	info << "Gzip files are not supported yet. Can not open MMCS file '";
	info << filename << "'\n";
	throw TracerException(info.str());
}
//------------------------------------------------------------------------------
void MmcsCorsikaSubBlockGetter::remember_MMCS_file_size_in_bytes() {
	if(FileTools::can_be_opened(filename))
		file_size_in_bytes = FileTools::size_in_bytes(filename);
	else
		throw_can_not_open_Mmcs_file("Failed to estimate file size");
}
//------------------------------------------------------------------------------
void MmcsCorsikaSubBlockGetter::open_MMCS_file() {
	MMCS_file.open (filename, std::ios::binary);
}
//------------------------------------------------------------------------------
void MmcsCorsikaSubBlockGetter::assert_MMCS_file_is_valid()const {
	assert_file_reading_is_fine(MMCS_file);
	assert_file_size_is_multiple_of_MMCS_block_size();
}
//------------------------------------------------------------------------------
void MmcsCorsikaSubBlockGetter::assert_file_reading_is_fine(const std::ifstream &file)const {
	if (!file.is_open())
		throw_can_not_open_Mmcs_file("fstream.is_open() returns: 'false'");
}
//------------------------------------------------------------------------------
void MmcsCorsikaSubBlockGetter::throw_can_not_open_Mmcs_file(const std::string details)const {
	std::stringstream info;
	info << "Can not open MMCS file: '" << filename << "'\n";
	info << details << "\n";
	throw TracerException( info.str());		
}
//------------------------------------------------------------------------------
MmcsCorsikaSubBlockGetter::~MmcsCorsikaSubBlockGetter() {
	MMCS_file.close();
}
//------------------------------------------------------------------------------
void MmcsCorsikaSubBlockGetter::assert_file_size_is_multiple_of_MMCS_block_size()const {
	if(file_size_in_bytes % block_size_in_bytes != 0) {
		std::stringstream info;
		info << "The Size of '" << filename;
		info << "' is not an exact multiple of the MMCS block size.\n";
		info << "File size : " << file_size_in_bytes << "byte\n";
		info << "Block size: " << block_size_in_bytes << "byte\n";
		throw TracerException(info.str());			
	}
}
//------------------------------------------------------------------------------
uint MmcsCorsikaSubBlockGetter::current_position_in_MMCS_file_in_bytes() {
	return MMCS_file.tellg();
}
//------------------------------------------------------------------------------
bool MmcsCorsikaSubBlockGetter::enough_MMCS_file_left_to_read_next_block() {
	return (file_size_left_in_bytes() >= block_size_in_bytes);
}
//------------------------------------------------------------------------------
uint MmcsCorsikaSubBlockGetter::file_size_left_in_bytes() {
	return 	file_size_in_bytes - current_position_in_MMCS_file_in_bytes();
}
//------------------------------------------------------------------------------
void MmcsCorsikaSubBlockGetter::read_next_block() {
	if(enough_MMCS_file_left_to_read_next_block()) {
		if(block_has_already_been_used())
			delete_old_data_in_block();

		block = read_block_from_MMCS_file();
		sub_block_in_block_counter = 0;
		block_counter++;
	}
}
//------------------------------------------------------------------------------
float* MmcsCorsikaSubBlockGetter::read_block_from_MMCS_file() {
	char *block_buffer = read_binary_block_from_MMCS_file();

	float *array_block;
	array_block = new float[block_size_in_words];

	memcpy(array_block, block_buffer ,block_size_in_words*sizeof(*array_block));

	delete[] block_buffer;
	return array_block;
}
//------------------------------------------------------------------------------
char* MmcsCorsikaSubBlockGetter::read_binary_block_from_MMCS_file() {
    char *block_buffer = new char [block_size_in_bytes];
    MMCS_file.read (block_buffer, block_size_in_bytes);
    return block_buffer;	
}
//------------------------------------------------------------------------------
void MmcsCorsikaSubBlockGetter::read_next_sub_block_from_block() {
	if(has_still_sub_blocks_left_on_block()) {
		copy_sub_block_from_block();
	}else{
		read_next_block();
		copy_sub_block_from_block();
	}
}
//------------------------------------------------------------------------------
bool MmcsCorsikaSubBlockGetter::has_still_sub_blocks_left_on_block()const {
	return (sub_block_in_block_counter < number_of_sub_blocks_in_block());
}
//------------------------------------------------------------------------------
uint MmcsCorsikaSubBlockGetter::number_of_sub_blocks_in_block()const {
	return block_size_in_words/SubBlock.size_in_words();
}
//------------------------------------------------------------------------------
void MmcsCorsikaSubBlockGetter::copy_sub_block_from_block() {

	for(uint i = start_for_sub_block_in_block_in_words(); 
		i < end_for_sub_block_in_block_in_words(); 
		i++ ) {
		SubBlock[i-start_for_sub_block_in_block_in_words()] = block[i];
	}

	sub_block_in_block_counter += 1;	
}
//------------------------------------------------------------------------------
uint MmcsCorsikaSubBlockGetter::start_for_sub_block_in_block_in_words()const{
	return sub_block_in_block_counter * SubBlock.size_in_words();
}
//------------------------------------------------------------------------------
uint MmcsCorsikaSubBlockGetter::end_for_sub_block_in_block_in_words()const{
	return start_for_sub_block_in_block_in_words() + SubBlock.size_in_words();
}
//------------------------------------------------------------------------------
MmcsCorsikaSubBlock MmcsCorsikaSubBlockGetter::get_next_sub_block() {
	read_next_sub_block_from_block();
	return SubBlock;
}
//------------------------------------------------------------------------------
bool MmcsCorsikaSubBlockGetter::block_has_already_been_used()const {
	return sub_block_in_block_counter > 0;
}
//------------------------------------------------------------------------------
void MmcsCorsikaSubBlockGetter::delete_old_data_in_block() {
	delete[] block;
}
//------------------------------------------------------------------------------
bool MmcsCorsikaSubBlockGetter::has_still_sub_blocks_left()const {
	return ( sum_of_sub_blocks_so_far() < total_number_of_sub_blocks() );
}
//------------------------------------------------------------------------------
uint MmcsCorsikaSubBlockGetter::total_number_of_sub_blocks()const {
	return file_size_in_bytes/SubBlock.size_in_bytes();
}
//------------------------------------------------------------------------------
uint MmcsCorsikaSubBlockGetter::sum_of_sub_blocks_so_far()const {
	return 	
		number_of_completed_blocks()*number_of_sub_blocks_in_block()+
		sub_block_in_block_counter;
}
//------------------------------------------------------------------------------
uint MmcsCorsikaSubBlockGetter::number_of_completed_blocks()const {
	return block_counter-1;
}
//------------------------------------------------------------------------------
void MmcsCorsikaSubBlockGetter::print() {
	std::cout << str();
}
//------------------------------------------------------------------------------
std::string MmcsCorsikaSubBlockGetter::str() {
	std::stringstream out;
	out << " MMCS_Sub_Block_Getter__________________________________________\n";
	out << "| file name..................... '" << filename << "'\n";	
	out << "| file size...................... " << file_size_in_bytes << " bytes\n";
	out << "| position....................... " << current_position_in_MMCS_file_in_bytes() << " bytes\n";
	out << "| file size left................. " << file_size_left_in_bytes() << " bytes\n";
	out << "| block size..................... " << block_size_in_bytes << " bytes / " << block_size_in_words << " words\n";
	out << "| sub block size................. " << SubBlock.size_in_bytes() << " bytes / " << SubBlock.size_in_words() << " words\n";
	out << "| number of sub blocks in block.. " << number_of_sub_blocks_in_block() << "\n";
	out << "| block counter.................. " << block_counter << "\n";
	out << "| number_of_completed_blocks..... " << number_of_completed_blocks() << "\n";
	out << "| sub block counter.............. " << sub_block_in_block_counter << "\n";
	out << "| sum of sub blocks so far....... " << sum_of_sub_blocks_so_far() << "\n";
	out << "| total number of sub blocks..... " << total_number_of_sub_blocks() << "\n";
	out << "|_______________________________________________________________\n";
	return out.str();	
}
//------------------------------------------------------------------------------
void MmcsCorsikaSubBlockGetter::print_sub_block()const {
	std::stringstream out;
	out << "sub block in block " << sub_block_in_block_counter << "/";
	out << number_of_sub_blocks_in_block() << " in block ";
	out << number_of_completed_blocks();
	out << "/" << file_size_in_bytes/block_size_in_bytes << " i.e. ";
	out << "sub block " << total_number_of_sub_blocks() << "/";
	out << sum_of_sub_blocks_so_far() << "\n";
	out << SubBlock.str() << "\n";
	std::cout << out.str();
}