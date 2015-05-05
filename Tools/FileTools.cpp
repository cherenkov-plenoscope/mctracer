#include "FileTools.h"
//------------------------------------------------------------------------------
bool FileTools::can_be_opened(const std::string &file_path) {
    if(FILE *file = fopen(file_path.c_str(), "r")) {
        fclose(file);
        return true;
    }else {
        return false;
    } 
}
//------------------------------------------------------------------------------
uint FileTools::size_in_bytes(const std::string name_of_file_to_get_size_of) {
	std::ifstream file;
	file.open(name_of_file_to_get_size_of, std::ios::binary | std::ios::ate);

	if (!file.is_open()) {
		file.close();
		std::stringstream info;
		info << "Toolbox: " << __func__ << "()\n";
		info << "Can not open file : '" << name_of_file_to_get_size_of;
		info << "' in order to get file size.\n";
		throw TracerException(info.str());
	}

	uint size_in_bytes = file.tellg();
	file.close();

	return size_in_bytes;
}
//------------------------------------------------------------------------------
void FileTools::write_text_to_file(
	const std::string &text,
	const std::string &path
) {
	std::ofstream text_file (path.c_str());
	if(text_file.is_open()) {
		text_file << text;
		text_file.close();
	}else{
		std::stringstream info;
		info << "FileTools: ";
		info << "Unable to write text to file: '" << path << "'.";
		throw TracerException(info.str());	  	
	}	
}