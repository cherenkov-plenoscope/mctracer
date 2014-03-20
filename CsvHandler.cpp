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
//======================================================================
/*
std::vector<std::string> CsvHandler::getNextLineAndSplitIntoTokens(
	std::istream& str)
{
	// Taken from 
	// http://stackoverflow.com/questions/1120140/csv-parser-in-c
    std::vector<std::string>   result;
    std::string                line;
    std::getline(str,line);

    std::stringstream          lineStream(line);
    std::string                cell;

    while(std::getline(lineStream,cell,','))
    {
        result.push_back(cell);
    }
    return result;
}*/