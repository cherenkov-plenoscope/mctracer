//=================================
// include guard
#ifndef __AsciiIo_H_INCLUDED__
#define __AsciiIo_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Tools/Tools.h"
#include "Tools/FileTools.h"


//=================================
namespace AsciiIo {
	
	std::vector<std::vector<double>> gen_table_from_file(const std::string &path);
	std::vector<std::vector<double>> gen_table_from_string(const std::string &text);
	void write_table_to_file(
		std::vector<std::vector<double>> table,
		const std::string &path
	);
	void write_table_to_file_with_header(
		std::vector<std::vector<double>> table,
		const std::string &path,
		const std::string &header
	);

	const char delimiter = '\t';
	const std::string delimiters_for_reading = "\t" " ";
	const char comment_escape = '#';
	const int precision = 10;

	std::string get_table_print(std::vector<std::vector<double>> &table);

	class TableReader {
		unsigned int current_row = 0;
		unsigned int current_col = 0;
		std::istringstream text;
		std::vector<std::vector<double>> table;
	public:
		TableReader(const std::string &text);
		std::vector<std::vector<double>> get_table()const;
	private:
		void fill_matrix_from_text();
		std::vector<double> text_row_2_numeric_row(
			const std::string &row
		);
		void push_back_token_to_numeric_row(
			const std::string token,
			std::vector<double> *numeric_row
		);
	};
}
#endif // __AsciiIo_H_INCLUDED__
