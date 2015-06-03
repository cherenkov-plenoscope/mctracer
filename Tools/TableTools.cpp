#include "TableTools.h"
namespace TableTools {
	//--------------------------------------------------------------------------
	const char Factory::delimiter = ',';
	//--------------------------------------------------------------------------
	Factory::Factory(const std::string new_path) : path(new_path){
		open_text_file();
		set_columns_based_on_first_row();
		fill_matrix_from_textfile();
		close_text_file();
	}
	//--------------------------------------------------------------------------
	void Factory::open_text_file() { 
		textfile.open(path.c_str());

		if(!textfile.is_open()) {
			std::stringstream info;
			info << "TableTools::Factory:\n";
			info << "Can not open file '" << path << "'.";
			throw TracerException(info.str());
		}
	}
	//--------------------------------------------------------------------------
	void Factory::close_text_file() {
		textfile.close();
	}
	//--------------------------------------------------------------------------
	void Factory::set_columns_based_on_first_row() {

		current_row = 0;
		std::string first_row;
		if(std::getline(textfile, first_row)) {
			column_names = StringTools::tokenize_text_given_delimiter_char(
				first_row,
				delimiter
			);
		}else{
			std::stringstream info;
			info << "TableTools::Factory:\n";
			info << "Can not read first row of file '" << path << "'.";
			throw TracerException(info.str());		
		}

		strip_whitespaces_from_column_names();
	}
	//--------------------------------------------------------------------------
	void Factory::strip_whitespaces_from_column_names() {
		for(uint coli=0; coli<column_names.size(); coli++) {
			column_names.at(coli) = 
				StringTools::strip_whitespaces(column_names.at(coli));
		}		
	}
	//--------------------------------------------------------------------------
	void Factory::fill_matrix_from_textfile() {

		std::string row;
		while(std::getline(textfile, row)) {
			current_row++;
			matrix.push_back(text_row_2_numeric_row(row));
		}
	}
	//--------------------------------------------------------------------------
	std::vector<double> Factory::text_row_2_numeric_row(
		const std::string &row
	) {
		const std::vector<std::string> tokens_in_row = 
			StringTools::tokenize_text_given_delimiter_char(row, delimiter);

		assert_cols_of_row_match_first_row_cols(tokens_in_row.size());
		std::vector<double> numeric_row;

		current_col = 0;
		for(std::string token : tokens_in_row)
			push_back_token_to_numeric_row(token, &numeric_row);

		return numeric_row;
	}
	//--------------------------------------------------------------------------
	void Factory::push_back_token_to_numeric_row(
		const std::string token,
		std::vector<double> *numeric_row
	) {
		current_col++;
		try{
			numeric_row->push_back(StrToDouble(token));
		}catch(TracerException &error) {
			std::stringstream info;
			info << "TableTools::Factory:\n";
			info << "Can not convert item '" << token << "' into a ";
			info << "floating point number. File '" << path << "' in row ";
			info << current_row << " and column " << current_col << ".\n";
			info << error.what();
			throw TracerException(info.str());
		}	
	}
	//--------------------------------------------------------------------------
	void Factory::assert_cols_of_row_match_first_row_cols(
		const uint current_cols
	)const {
		if(current_cols != cols()) {
			std::stringstream info;
			info << "TableTools::Factory:\n";
			info << "Expected every row in file '" << path << "' ";
			info << "to have exactly " << cols() << " columns, ";
			info << "but actual row number " << current_row << " ";
			info << "has " << current_cols << " columns. ";
			info << "Expect a rectangular matrix structure. ";
			info << "The number of columns ";
			info << "is defined by the first row in the file.";
			throw TracerException(info.str());		
		}
	}
	//--------------------------------------------------------------------------
	uint Factory::cols()const {
		return column_names.size();
	}
	//--------------------------------------------------------------------------
	uint Factory::rows()const {
		return matrix.size();
	}
	//--------------------------------------------------------------------------
	std::valarray<std::valarray<double>> Factory::get_table()const {

		std::valarray<std::valarray<double>> table(cols());

		for(uint coli=0; coli<cols(); coli++) {
			std::valarray<double> column(rows());

			for(uint rowi=0; rowi<rows(); rowi++)
				column[rowi] = matrix[rowi][coli];

			table[coli] = column;
		}

		return table;
	}
	//--------------------------------------------------------------------------
	std::vector<std::string> Factory::get_column_names()const {
		return column_names;
	}
	//--------------------------------------------------------------------------
	std::string get_table_print(
		const std::valarray<std::valarray<double>> &table
	) {
		std::vector<std::string> empty_column_names;
		return TableTools::get_table_and_column_name_print(
			table,
			empty_column_names
		);
	}
	//--------------------------------------------------------------------------
	std::string get_table_and_column_name_print(
		const std::valarray<std::valarray<double>> &table,
		const std::vector<std::string> &column_names
	) {
		if(table.size() == 0)
			return "";

		if(table[0].size() == 0)
			return "";

		const uint rows = table[0].size();
		const uint cols = table.size();

		std::stringstream out;
		out.precision(10);

		if(column_names.size()>0) {
			for(uint col=0; col<column_names.size()-1; col++){
				out << column_names.at(col) << ", ";
			}
			out << column_names.back() << "\n";
		}

		for(uint row=0; row<rows; row++) {
			for(uint col=0; col<cols-1; col++) {
				out << table[col][row] << ", ";
			}
			out << table[cols-1][row] << "\n";
		}

		return out.str();
	}
	//--------------------------------------------------------------------------
	std::string get_column_print(const std::valarray<double> &column) {

		std::stringstream out;
		out.precision(10);

		for(uint row=0; row<column.size(); row++)
			out << column[row] << "\n";

		return out.str();
	}
	//--------------------------------------------------------------------------
	std::valarray<std::valarray<double>> read_file(const std::string &path) {
		Factory fab(path);
		return fab.get_table();
	}
	//--------------------------------------------------------------------------
	void write_table_to_file(
		const std::valarray<std::valarray<double>> &table, 
		const std::string &path
	) {
		std::vector<std::string> empty_column_names;
		TableTools::write_table_and_column_names_to_file(
			table,
			empty_column_names,
			path
		);	
	}
	//--------------------------------------------------------------------------
	void write_table_and_column_names_to_file(
		const std::valarray<std::valarray<double>> &table,
		const std::vector<std::string> &column_names,
		const std::string &path
	) {
		std::ofstream text_file (path.c_str());
		if(text_file.is_open()) {
			text_file << TableTools::get_table_and_column_name_print(
				table, 
				column_names
			);
			text_file.close();
		}else{
			std::stringstream info;
			info << "TableTools: ";
			info << "Unable to write table to file: '" << path << "'.";
			throw TracerException(info.str());	  	
		}	
	}
}