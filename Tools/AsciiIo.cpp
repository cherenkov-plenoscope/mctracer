#include "AsciiIo.h"
#include "Tools/StringTools.h"
using std::stringstream;

namespace AsciiIo {
	//--------------------------------------------------------------------------
	vector<vector<double>> gen_table_from_file(const string &path) {
		TableReader reader(path);
		return reader.get_table();
	}
	//--------------------------------------------------------------------------
	void write_table_to_file_with_header(
		vector<vector<double>> table,
		const string &path,
		const string &header
	) {
		stringstream out;
		out << StringTools::place_first_infront_of_each_new_line_of_second(
				"# ", header
		);
		out << get_table_print(table);
		FileTools::write_text_to_file(out.str(), path);		
	}
	//--------------------------------------------------------------------------
	void write_table_to_file(
		vector<vector<double>> table,
		const string &path
	) {
		FileTools::write_text_to_file(get_table_print(table), path);		
	}
	//--------------------------------------------------------------------------
	string get_table_print(vector<vector<double>> &table) {
		stringstream out;
		out.precision(precision);

		for(uint row=0; row<table.size(); row++) {
			for(uint col=0; col<table.at(row).size()-1; col++) {
				out << table[row][col] << delimiter;
			}
			out << table[row][table.at(row).size()-1];
			
			if(row !=table.size()) 
				out << "\n";
		}
		return out.str();		
	}
	//--------------------------------------------------------------------------
	TableReader::TableReader(const string &_path) {
		path = _path;
		open_text_file();
		fill_matrix_from_textfile();
		close_text_file();
	}
	//--------------------------------------------------------------------------
	vector<vector<double>> TableReader::get_table()const {
		return table;
	}
	//--------------------------------------------------------------------------
	void TableReader::open_text_file() {
		textfile.open(path.c_str());

		if(!textfile.is_open()) {
			stringstream info;
			info << "TableReader::open_text_file\n";
			info << "Can not open file '" << path << "'.";
			throw TracerException(info.str());
		}
	}
	//--------------------------------------------------------------------------
	void TableReader::fill_matrix_from_textfile() {
		string row;
		while(std::getline(textfile, row)) {
			row = StringTools::strip_whitespaces(row);
			current_row++;
			if(row.length()!=0)
				if(*row.begin() != comment_escape)
					table.push_back(text_row_2_numeric_row(row));
		}
	}
	//--------------------------------------------------------------------------
	vector<double> TableReader::text_row_2_numeric_row(const string &row) {
		const vector<string> tokens_in_row = 
			StringTools::tokenize_text_using_either_one_of_delimiters(
				row, 
				delimiters_for_reading
			);

		vector<double> numeric_row;

		current_col = 0;
		for(string token : tokens_in_row)
			push_back_token_to_numeric_row(token, &numeric_row);

		return numeric_row;
	}
	//--------------------------------------------------------------------------
	void TableReader::push_back_token_to_numeric_row(
		const string token,
		vector<double> *numeric_row
	) {
		current_col++;
		try{
			numeric_row->push_back(StringTools::to_double(token));
		}catch(TracerException &error) {
			stringstream info;
			info << "TableReader::push_back_token_to_numeric_row:\n";
			info << "Can not convert item '" << token << "' into a ";
			info << "floating point number. File '" << path << "' in row ";
			info << current_row << " and column " << current_col << ".\n";
			info << error.what();
			throw TracerException(info.str());
		}	
	}
	//--------------------------------------------------------------------------
	void TableReader::close_text_file() {
		textfile.close();
	}
}