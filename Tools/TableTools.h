//=================================
// include guard
#ifndef __Factory_H_INCLUDED__
#define __Factory_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream> 
#include <string>
#include <sstream>
#include <valarray>
#include <math.h>
#include <fstream>
#include "TracerException.h"
#include "Tools/Tools.h"

//=================================
namespace TableTools {
	struct Table {
		std::valarray<std::valarray<double>> table;
		std::vector<std::string> column_names;
	};

	std::valarray<std::valarray<double>> read_file(const std::string &path);
	void write_table_to_file(
		const std::valarray<std::valarray<double>> &table, 
		const std::string path
	);
	void write_table_and_column_names_to_file(
		const std::valarray<std::valarray<double>> &table,
		const std::vector<std::string> &column_names,
		const std::string &path
	);
	std::string get_table_print(
		const std::valarray<std::valarray<double>> &table
	);
	std::string get_table_and_column_name_print(
		const std::valarray<std::valarray<double>> &table,
		const std::vector<std::string> &column_names
	); 
	std::string get_column_print(const std::valarray<double> &column);

	class Factory {
		std::vector<std::vector<double>> matrix;
		std::vector<std::string> column_names;
		const std::string path;

		std::ifstream textfile;

		uint current_row;
		uint current_col;

		static const char delimiter;
	public:
		Factory(const std::string new_path);
		std::valarray<std::valarray<double>> get_table()const;
		std::vector<std::string> get_column_names()const;
	private:
		void open_text_file();
		void close_text_file();
		void fill_matrix_from_textfile();
		void set_columns_based_on_first_row();
		std::vector<double> text_row_2_numeric_row(const std::string &row);
		void assert_cols_of_row_match_first_row_cols(
			const uint current_cols
		)const;
		void init_matrix_columns();
		uint cols()const;
		uint rows()const;
		void push_back_token_to_numeric_row(
			const std::string token,
			std::vector<double> *numeric_row
		);
		void strip_whitespaces_from_column_names();
	};
}
#endif // __Factory_H_INCLUDED__
