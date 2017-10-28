//=================================
// include guard
#ifndef __HeaderBlockInOut_H_INCLUDE__
#define __HeaderBlockInOut_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies 
#include <string>
#include <array>
#include <vector>

namespace HeaderBlock {
	void write(std::array<float, 273> block, const std::string &path);
	void write(std::vector<std::array<float, 273>> blocks, const std::string &path);
	std::vector<std::array<float, 273>> read(const std::string &path);
}

#endif // __HeaderBlockInOut_H_INCLUDE__
