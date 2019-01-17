// Copyright 2014 Sebastian A. Mueller
#include "Xml/Problem.h"
#include <math.h>
#include <sstream>
#include <iomanip>
#include "Xml/Exceptions.h"
using std::string;

namespace Xml  {

Problem::Problem(
    const std::shared_ptr<string> _file,
    const int _offset,
    const string _path):
    file(_file),
    offset(_offset),
    path(_path) {
    init();
}

Problem::Problem(Node node):
    file(node.file),
    offset(node.node.offset_debug()),
    path(node.path) {
    init();
}

void Problem::init() {
    locate_new_lines_in_xml_file();
    locate_problem_column_and_line_using_new_lines();
}

unsigned int Problem::get_line()const {
    return line;
}

unsigned int Problem::get_column()const {
    return column;
}

string Problem::get_path()const {
    return path;
}

void Problem::locate_new_lines_in_xml_file() {
    new_lines.clear();
    for (unsigned int offset = 0; offset < file->length(); offset++)
        if (file->at(offset++) == '\n')
            new_lines.push_back(offset);
}

void Problem::locate_problem_column_and_line_using_new_lines() {
    std::vector<ptrdiff_t>::const_iterator it =
        std::lower_bound(new_lines.begin(), new_lines.end(), offset);

    size_t index = it - new_lines.begin();

    line = 1 + index;
    column = index == 0 ? offset + 1 : offset - new_lines[index - 1];
}

string Problem::get_problem_section_from_original_file()const {
    std::stringstream problem_section;
    std::istringstream ifile(*file);
    string line_of_text;

    const unsigned int line_radius = 5;
    const unsigned int max_digits_for_row_number =
        ceil(log10(line+line_radius));

    unsigned int current_line_number = 0;
    while (std::getline(ifile, line_of_text)) {
        std::stringstream line_prefix;
        line_prefix << std::setw(max_digits_for_row_number);
        line_prefix << current_line_number;

        if (line+1 == current_line_number)
            line_prefix << "|-> ";
        else
            line_prefix << "|   ";

        if (
            abs(
                static_cast<int>(current_line_number) -
                static_cast<int>(line))
            < static_cast<int>(line_radius))
            problem_section << line_prefix.str() << line_of_text << std::endl;

        current_line_number++;
    }

    return problem_section.str();
}

}  // namespace Xml
