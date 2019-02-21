// Copyright 2014 Sebastian A. Mueller
#ifndef TOOLS_ASCIIIO_H_
#define TOOLS_ASCIIIO_H_

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Tools/Tools.h"
#include "Tools/FileTools.h"

namespace relleums {
namespace AsciiIo {

std::vector<std::vector<double>> gen_table_from_file(
    const std::string &path);
std::vector<std::vector<double>> gen_table_from_string(
    const std::string &text);
void write_table_to_file(
    std::vector<std::vector<double>> table,
    const std::string &path);
void write_table_to_file_with_header(
    std::vector<std::vector<double>> table,
    const std::string &path,
    const std::string &header);
const char delimiter = '\t';
const char delimiters_for_reading[] = "\t" " ";
const char comment_escape = '#';
const int precision = 10;
std::string get_table_print(const std::vector<std::vector<double>> &table);

class TableReader {
    unsigned int current_row = 0;
    unsigned int current_col = 0;
    std::istringstream text;
    std::vector<std::vector<double>> table;
 public:
    explicit TableReader(const std::string &text);
    std::vector<std::vector<double>> get_table()const;
 private:
    void fill_matrix_from_text();
    std::vector<double> text_row_2_numeric_row(
        const std::string &row);
    void push_back_token_to_numeric_row(
        const std::string token,
        std::vector<double> *numeric_row);
};

}  // namespace AsciiIo
}  // namespace relleums

#endif  // TOOLS_ASCIIIO_H_
