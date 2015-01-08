#include "FileTools.h"
//--------------------------------------------------------------------------
bool FileTools::can_be_opened(const std::string &file_path) {
    if(FILE *file = fopen(file_path.c_str(), "r")) {
        fclose(file);
        return true;
    }else {
        return false;
    } 
}
//--------------------------------------------------------------------------
uint FileTools::size_in_bytes(const std::string name_of_file_to_get_size_of) {
	std::ifstream file;
	file.open(name_of_file_to_get_size_of, std::ios::binary | std::ios::ate);

	if (!file.is_open()) {
		std::stringstream info;
		info << "Toolbox: " << __func__ << "()\n";
		info << "Can not open file : '" << name_of_file_to_get_size_of;
		info << "' in order to get file size.\n";
		throw TracerException(info.str());	
	}
	return file.tellg();	
}