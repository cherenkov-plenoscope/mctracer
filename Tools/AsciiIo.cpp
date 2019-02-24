// Copyright 2014 Sebastian A. Mueller
#include "Tools/AsciiIo.h"
#include <math.h>
#include <sstream>
#include <string>
#include "Core/mctracer.h"
using std::stringstream;
using std::string;
using std::vector;

namespace relleums {
namespace AsciiIo {

vector<vector<double>> gen_table_from_file(const string &path) {
    std::ifstream textfile;
    textfile.open(path.c_str());

    if (!textfile.is_open()) {
        stringstream info;
        info << "AsciiIo::gen_table_from_file\n";
        info << "Can not open file '" << path << "'.";
        throw std::runtime_error(info.str());
    }

    string text_in_file;
    textfile.seekg(0, std::ios::end);
    text_in_file.reserve(textfile.tellg());
    textfile.seekg(0, std::ios::beg);

    text_in_file.assign(
        (std::istreambuf_iterator<char>(textfile)),
        std::istreambuf_iterator<char>());
    textfile.close();

    TableReader reader(text_in_file);
    return reader.get_table();
}

vector<vector<double>> gen_table_from_string(const string &text) {
    TableReader reader(text);
    return reader.get_table();
}

void write_table_to_file_with_header(
    vector<vector<double>> table,
    const string &path,
    const string &header
) {
    stringstream out;
    out << txt::place_first_infront_of_each_new_line_of_second(
            "# ", header);
    out << get_table_print(table);
    FileTools::write_text_to_file(out.str(), path);
}

void write_table_to_file(
    vector<vector<double>> table,
    const string &path
) {
    FileTools::write_text_to_file(get_table_print(table), path);
}

string get_table_print(const vector<vector<double>> &table) {
    stringstream out;
    out.precision(precision);

    for (unsigned int row = 0; row < table.size(); row++) {
        for (unsigned int col = 0; col < table.at(row).size() - 1; col++) {
            out << table[row][col] << delimiter;
        }
        out << table[row][table.at(row).size()-1];

        if (row !=table.size())
            out << "\n";
    }
    return out.str();
}

TableReader::TableReader(const string &_text):text(_text) {
    fill_matrix_from_text();
}

vector<vector<double>> TableReader::get_table()const {
    return table;
}

void TableReader::fill_matrix_from_text() {
    string row;
    while (std::getline(text, row)) {
        row = txt::strip_whitespaces(row);
        current_row++;
        if (row.length()!= 0)
            if (*row.begin() != comment_escape)
                table.push_back(text_row_2_numeric_row(row));
    }
}

vector<double> TableReader::text_row_2_numeric_row(const string &row) {
    const vector<string> tokens_in_row =
        txt::tokenize_text_using_either_one_of_delimiters(
            row,
            delimiters_for_reading);

    vector<double> numeric_row;

    current_col = 0;
    for (string token : tokens_in_row)
        push_back_token_to_numeric_row(token, &numeric_row);

    return numeric_row;
}

void TableReader::push_back_token_to_numeric_row(
    const string token,
    vector<double> *numeric_row
) {
    current_col++;
    try {
        numeric_row->push_back(txt::to_double(token));
    } catch(std::invalid_argument &error) {
        stringstream info;
        info << "TableReader::push_back_token_to_numeric_row:\n";
        info << "Can not convert item '" << token << "' into a ";
        info << "floating point number in row ";
        info << current_row << " and column " << current_col << ".\n";
        info << error.what();
        throw std::invalid_argument(info.str());
    }
}

}  // namespace AsciiIo
}  // namespace relleums
