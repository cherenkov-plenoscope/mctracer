//=================================
// include guard
#ifndef __AsciiIo_H_INCLUDED__
#define __AsciiIo_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream> 
#include <string>
#include <sstream>
#include <vector>
#include <math.h>
#include <fstream>
#include "TracerException.h"
#include "Tools/Tools.h"
#include "Tools/FileTools.h"

//=================================
namespace AsciiIo {
	
	std::vector<std::vector<double>> gen_table_from_file(
		const std::string &path
	);

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
		uint current_row = 0;
		uint current_col = 0;
		std::string path;
		std::ifstream textfile;
		std::vector<std::vector<double>> table;
	public:
		TableReader(const std::string &path);
		std::vector<std::vector<double>> get_table()const;
	private:
		void open_text_file();
		void fill_matrix_from_textfile();
		void close_text_file();
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
