// Copyright 2014 Sebastian A. Mueller
#ifndef XML_PROBLEM_H_
#define XML_PROBLEM_H_

#include <memory>
#include <vector>
#include <string>
#include "Xml/pugi/pugixml.hpp"
#include "Xml/Node.h"

namespace Xml {

class Problem {
    const std::shared_ptr<std::string> file;
    const int offset;
    const std::string path;

    std::vector<ptrdiff_t> new_lines;
    unsigned int column;
    unsigned int line;
 public:
    Problem(
        const std::shared_ptr<std::string> _file,
        const int _offset,
        const std::string _path);
    explicit Problem(Node node);
    std::string get_problem_section_from_original_file()const;
    unsigned int get_line()const;
    unsigned int get_column()const;
    std::string get_path()const;
 private:
    void init();
    void locate_new_lines_in_xml_file();
    void locate_problem_column_and_line_using_new_lines();
};

}  // namespace Xml

#endif  // XML_PROBLEM_H_
