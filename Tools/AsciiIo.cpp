#include "AsciiIo.h"
#include "Tools/StringTools.h"
namespace AsciiIo {
	//--------------------------------------------------------------------------
	std::vector<std::vector<double>> gen_table_from_file(
		const std::string &path
	) {
		TableReader reader(path);
		return reader.get_table();
	}
	//--------------------------------------------------------------------------
	void write_table_to_file(
		std::vector<std::vector<double>> table,
		const std::string &path
	) {
		FileTools::write_text_to_file(get_table_print(table), path);		
	}
	//--------------------------------------------------------------------------
	std::string get_table_print(std::vector<std::vector<double>> &table) {
		std::stringstream out;
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
	TableReader::TableReader(const std::string &_path) {
		path = _path;
		open_text_file();
		fill_matrix_from_textfile();
		close_text_file();
	}
	//--------------------------------------------------------------------------
	std::vector<std::vector<double>> TableReader::get_table()const {
		return table;
	}
	//--------------------------------------------------------------------------
	void TableReader::open_text_file() {
		textfile.open(path.c_str());

		if(!textfile.is_open()) {
			std::stringstream info;
			info << "TableReader::open_text_file\n";
			info << "Can not open file '" << path << "'.";
			throw TracerException(info.str());
		}
	}
	//--------------------------------------------------------------------------
	void TableReader::fill_matrix_from_textfile() {
		std::string row;
		while(std::getline(textfile, row)) {
			row = StringTools::strip_whitespaces(row);
			current_row++;
			if(row.length()!=0)
				if(*row.begin() != comment_escape)
					table.push_back(text_row_2_numeric_row(row));
		}
	}
	//--------------------------------------------------------------------------
	std::vector<double> TableReader::text_row_2_numeric_row(
		const std::string &row
	) {
		const std::vector<std::string> tokens_in_row = 
			StringTools::tokenize_text_given_delimiter_char(row, delimiter);

		std::vector<double> numeric_row;

		current_col = 0;
		for(std::string token : tokens_in_row)
			push_back_token_to_numeric_row(token, &numeric_row);

		return numeric_row;
	}
	//--------------------------------------------------------------------------
	void TableReader::push_back_token_to_numeric_row(
		const std::string token,
		std::vector<double> *numeric_row
	) {
		current_col++;
		try{
			numeric_row->push_back(StrToDouble(token));
		}catch(TracerException &error) {
			std::stringstream info;
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