#include "CsvHandler.h"
//======================================================================
CsvHandler::CsvHandler(std::string new_csv_file_name){
	csv_file_name = new_csv_file_name;
}
//======================================================================
void CsvHandler::write(std::string csv_table_to_export)const{
	ofstream myfile (csv_file_name.c_str());
	if (myfile.is_open())
	{
		myfile << csv_table_to_export;
		myfile.close();
	}	
}
