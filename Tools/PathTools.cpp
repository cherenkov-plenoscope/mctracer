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

    Path::Path(const string full_path) {

        full = full_path;
        int last_dot_in_path = full_path.find_last_of(".");
        int last_delimiter_in_path = full_path.find_last_of("/\\");

        if(last_dot_in_path != -1 && last_dot_in_path>last_delimiter_in_path)
            extension = full_path.substr(last_dot_in_path + 1, full_path.length());

        if(last_delimiter_in_path != -1)
            path = full_path.substr(0, last_delimiter_in_path);

        const int point_length = (extension.length()>0?  1: 0);
        const int delimiter_length = (path.length()>0? 1: 0);  
        filename = full_path.substr(
            path.length()+delimiter_length, 
            full_path.length()-path.length()-delimiter_length-extension.length()-point_length
        );
    }
}//PathTools