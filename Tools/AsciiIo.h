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
#include "Tools/Tools.h"
#include "Tools/FileTools.h"
using std::string;
using std::vector;

//=================================
namespace AsciiIo {
	
	vector<vector<double>> gen_table_from_file(const string &path);
	vector<vector<double>> gen_table_from_string(const string &text);
	void write_table_to_file(
		vector<vector<double>> table,
		const string &path
	);
	void write_table_to_file_with_header(
		vector<vector<double>> table,
		const string &path,
		const string &header
	);

	const char delimiter = '\t';
	const string delimiters_for_reading = "\t" " ";
	const char comment_escape = '#';
	const int precision = 10;

	string get_table_print(vector<vector<double>> &table);

	class TableReader {
		uint current_row = 0;
		uint current_col = 0;
		std::istringstream text;
		vector<vector<double>> table;
	public:
		TableReader(const string &text);
		vector<vector<double>> get_table()const;
	private:
		void fill_matrix_from_text();
		vector<double> text_row_2_numeric_row(
			const string &row
		);
		void push_back_token_to_numeric_row(
			const string token,
			vector<double> *numeric_row
		);
	};
}
#endif // __AsciiIo_H_INCLUDED__
