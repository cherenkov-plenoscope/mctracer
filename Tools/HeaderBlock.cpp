#include "HeaderBlock.h"
#include <fstream>
#include <sstream>
using std::string;
using std::vector;
using std::array;

namespace HeaderBlock {
//------------------------------------------------------------------------------
void write(array<float, 273> block, const string &path) {

    vector<array<float, 273>> blocks;
    blocks.push_back(block);
    write(blocks, path);
}
//------------------------------------------------------------------------------
void write(vector<array<float, 273>> blocks, const string &path) {
    
    std::ofstream file;
    file.open(path, std::ios::out | std::ios::binary);
    if(!file.is_open()) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "HeaderBlock: Unable to write file: '" << path << "'\n";
        throw std::runtime_error(info.str());      
    }

    for(const array<float, 273> &block: blocks)
        file.write((char*)&block, sizeof(array<float, 273>));

    file.close();
}
//------------------------------------------------------------------------------
vector<array<float, 273>> read(const string &path) {
    
    std::ifstream file;
    file.open(path, std::ios::binary);
    if(!file.is_open()) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "HeaderBlock: Unable to read file: '" << path << "'\n";
        throw std::runtime_error(info.str());      
    }

    vector<array<float, 273>> blocks;
    while(true) {
        array<float, 273> block;
        file.read((char*)&block, sizeof(array<float, 273>));
        if(!file.eof())
            blocks.push_back(block);
        else
            break;
    }
    file.close();
    return blocks;
}
//------------------------------------------------------------------------------
}//HeaderBlock