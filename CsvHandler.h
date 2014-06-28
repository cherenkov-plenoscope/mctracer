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
#include <vector>
using namespace std;
// http://stackoverflow.com/questions/1120140/csv-parser-in-c
class CsvRow{
private:
	vector<string> tokenized_csv_row;
public:
	string const& operator[](size_t index)const;
	void push_back(string new_token);
	size_t size()const;
	bool ReadNextRow(istream& stream);
	string WriteNextRow(ostream& stream)const;
	void append(CsvRow row_to_append);
	friend istream& operator >> (istream& str,CsvRow& data);
	friend ostream& operator << (ostream& str,CsvRow& data);
};
//=================================
/*class CsvHandler{
	string csv_file_name;

public:
	CsvHandler(std::string new_csv_file_name);
	void write(std::string csv_table_to_export)const;
};*/
#endif // __CSVHANDLER_H_INCLUDED__
