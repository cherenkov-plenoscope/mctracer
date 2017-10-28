//=================================
// include guard
#ifndef __HeaderBlockInOut_H_INCLUDE__
#define __HeaderBlockInOut_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies 
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <fstream>
using std::string;
using std::vector;
using std::array;

namespace HeaderBlock {
	void write(array<float, 273> block, const string &path);
	void write(vector<array<float, 273>> blocks, const string &path);
	vector<array<float, 273>> read(const string &path);
}

#endif // __HeaderBlockInOut_H_INCLUDE__
