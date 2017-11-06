#include "MmcsCorsikaTools.h"
//------------------------------------------------------------------------------
namespace MmcsCorsikaTools {
	//--------------------------------------------------------------------------
	float str2float_4byte_bin_map(const std::string word) {

		if(word.size() != 4) {
			std::stringstream info;
			info << "Toolbox: " << __func__ << "()\n";
			info << "Expected word size: 4 bytes, but actual: ";
			info << word.size() << " bytes.\n";
			info << "word: '" << word << "'.\n";
			throw std::runtime_error(info.str());			
		}

		char word_array[] = {word.at(0), word.at(1), word.at(2), word.at(3)};
		float f;
		memcpy(&f, &word_array, sizeof(f));
		return f;
	}
	//--------------------------------------------------------------------------
	std::string float2str_4byte_bin_map(const float word_in_float) {

		char word_array[4];

		memcpy(&word_array, &word_in_float, sizeof(word_array));

		std::string word;
		for(unsigned int i=0; i<4; i++)
			word += word_array[i];

		return word;
	}
	//--------------------------------------------------------------------------
	std::string float_vec_2_str_using_delimiter(
		const std::vector<float> vec, 
		const std::string delimiter
	) {
		std::stringstream out;
		for(float element : vec) 
			out << element << delimiter;
		return out.str();
	}
	//--------------------------------------------------------------------------
}