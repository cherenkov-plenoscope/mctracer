// Copyright 2014 Sebastian A. Mueller
#include "merlict/tsvio.h"
#include <math.h>
#include <sstream>
#include <string>
#include "merlict/txt.h"
#include "merlict/tools.h"


namespace merlict {
namespace tsvio {

std::vector<std::vector<double>> gen_table_from_file(const std::string &path) {
    std::ifstream textfile;
    textfile.open(path.c_str());

    if (!textfile.is_open()) {
        std::stringstream info;
        info << "tsvio::gen_table_from_file\n";
        info << "Can not open file '" << path << "'.";
        throw std::runtime_error(info.str());
    }

    std::string text_in_file;
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

std::vector<std::vector<double>> gen_table_from_string(
    const std::string &text
) {
    TableReader reader(text);
    return reader.get_table();
}

void write_table_to_file_with_header(
    std::vector<std::vector<double>> table,
    const std::string &path,
    const std::string &header
) {
    std::stringstream out;
    out << txt::place_first_infront_of_each_new_line_of_second(
            "# ", header);
    out << get_table_print(table);
    write_text_to_file(out.str(), path);
}

void write_table_to_file(
    std::vector<std::vector<double>> table,
    const std::string &path
) {
    write_text_to_file(get_table_print(table), path);
}

std::string get_table_print(const std::vector<std::vector<double>> &table) {
    std::stringstream out;
    out.precision(FLOAT_PRECISION);

    for (unsigned int row = 0; row < table.size(); row++) {
        for (unsigned int col = 0; col < table.at(row).size() - 1; col++) {
            out << table[row][col] << DELIMITER;
        }
        out << table[row][table.at(row).size()-1];

        if (row !=table.size())
            out << "\n";
    }
    return out.str();
}

TableReader::TableReader(const std::string &_text):text(_text) {
    fill_matrix_from_text();
}

std::vector<std::vector<double>> TableReader::get_table()const {
    return table;
}

void TableReader::fill_matrix_from_text() {
    std::string row;
    while (std::getline(text, row)) {
        row = txt::strip_whitespaces(row);
        current_row++;
        if (row.length()!= 0)
            if (*row.begin() != COMMENT_ESCAPE)
                table.push_back(text_row_2_numeric_row(row));
    }
}

std::vector<double> TableReader::text_row_2_numeric_row(
    const std::string &row
) {
    const std::vector<std::string> tokens_in_row =
        txt::tokenize_text_using_either_one_of_delimiters(
            row,
            DELIMITERS_FOR_READING);

    std::vector<double> numeric_row;

    current_col = 0;
    for (std::string token : tokens_in_row)
        push_back_token_to_numeric_row(token, &numeric_row);

    return numeric_row;
}

void TableReader::push_back_token_to_numeric_row(
    const std::string token,
    std::vector<double> *numeric_row
) {
    current_col++;
    try {
        numeric_row->push_back(txt::to_double(token));
    } catch(std::invalid_argument &error) {
        std::stringstream info;
        info << "TableReader::push_back_token_to_numeric_row:\n";
        info << "Can not convert item '" << token << "' into a ";
        info << "floating point number in row ";
        info << current_row << " and column " << current_col << ".\n";
        info << error.what();
        throw std::invalid_argument(info.str());
    }
}

}  // namespace tsvio
}  // namespace merlict
