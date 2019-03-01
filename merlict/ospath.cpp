// Copyright 2014 Sebastian A. Mueller
#include "merlict/ospath.h"
#include <sstream>
#include "merlict/txt.h"


namespace merlict {
namespace ospath {

    std::string join(std::string path1, std::string path2) {
        std::stringstream out;
        if (txt::is_ending(path1, "/"))
            out << path1;
        else if (!path1.empty())
            out << path1 << "/";
        if (path2.find_last_of("/") == 0)
            out << path2.substr(1, path2.length());
        else
            out << path2;

        return out.str();
    }

    Path::Path(const std::string full_path) {
        path = full_path;
        int last_dot_in_path = full_path.find_last_of(".");
        int last_delimiter_in_path = full_path.find_last_of("/\\");
        if (last_dot_in_path != -1 && last_dot_in_path > last_delimiter_in_path)
            extension =
                full_path.substr(last_dot_in_path + 1, full_path.length());
        if (last_delimiter_in_path != -1)
            dirname = full_path.substr(0, last_delimiter_in_path);
        const int point_length = (extension.length() > 0 ? 1 : 0);
        const int delimiter_length = (dirname.length() > 0 ? 1 : 0);
        basename_wo_extension = full_path.substr(
            dirname.length()+
            delimiter_length,
            full_path.length()-
            dirname.length()-
            delimiter_length-extension.length()-
            point_length);
        if (extension.empty())
            basename = basename_wo_extension;
        else
            basename = basename_wo_extension +"."+ extension;
    }

    Path::Path() {
        *this = Path("");
    }

}  // namespace ospath
}  // namespace merlict
