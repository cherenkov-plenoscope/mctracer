#include "PathTools.h"

namespace PathTools {
    
    FullPath split_path_and_filename(std::string path) {

        FullPath fp;
        int last_delimiter_in_path = path.find_last_of("/\\");

        if(last_delimiter_in_path == -1)
            fp.path = "";
        else
            fp.path = path.substr(0, last_delimiter_in_path + 1); 

        fp.filename = path.substr(last_delimiter_in_path + 1);
        
        return fp;      
    }
}