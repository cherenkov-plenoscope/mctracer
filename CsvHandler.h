//=================================
// include guard
#ifndef __CSVHANDLER_H_INCLUDED__
#define __CSVHANDLER_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

//=================================
class CsvHandler{
	std::string csv_file_name;
//=================================	
public:
CsvHandler(std::string new_csv_file_name);
void write(std::string csv_table_to_export)const;
private:
//getNextLineAndSplitIntoTokens(std::istream& str)const;
};
#endif // __CSVHANDLER_H_INCLUDED__
