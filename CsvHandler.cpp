#include "CsvHandler.h"
//======================================================================
string const& CsvRow::operator[](size_t index)const{
	return tokenized_csv_row[index];
}
//----------------------------------------------------------------------
void CsvRow::push_back(string new_token){
	tokenized_csv_row.push_back(new_token);
}
//----------------------------------------------------------------------
size_t CsvRow::size()const{
	return tokenized_csv_row.size();
}
//----------------------------------------------------------------------
void CsvRow::ReadNextRow(istream& stream){
	string csv_line;
	getline(stream,csv_line);

	stringstream lineStream(csv_line);
	string token;

	tokenized_csv_row.clear();
	while(getline(lineStream,token,',')){
		tokenized_csv_row.push_back(token);
	}
}
//----------------------------------------------------------------------
string CsvRow::WriteNextRow(ostream& stream)const{
	stringstream csv_line;

	for(size_t i=0; i < tokenized_csv_row.size(); i++){
		
		csv_line << tokenized_csv_row.at(i);

		if((i+1) != tokenized_csv_row.size()){
			csv_line << ",";
		}
	}

	csv_line << "\n";

	return csv_line.str();
}
//----------------------------------------------------------------------
void CsvRow::append(CsvRow row_to_append){
	//http://stackoverflow.com/questions/2551775/c-appending-a-vector-to-a-vector
	tokenized_csv_row.insert(
		tokenized_csv_row.end(),
		row_to_append.tokenized_csv_row.begin(),
		row_to_append.tokenized_csv_row.end()
	);
}
//----------------------------------------------------------------------
istream& operator >> (istream& str,CsvRow& data)
{
    data.ReadNextRow(str);
    return str;
} 
//----------------------------------------------------------------------
ostream& operator << (ostream& str,CsvRow& data)
{
	str << data.WriteNextRow(str);
    return str;
} 
//======================================================================
//
//======================================================================
/*
CsvHandler::CsvHandler(std::string new_csv_file_name){
	csv_file_name = new_csv_file_name;
}
//----------------------------------------------------------------------
void CsvHandler::write(std::string csv_table_to_export)const{
	ofstream myfile (csv_file_name.c_str());
	if (myfile.is_open())
	{
		myfile << csv_table_to_export;
		myfile.close();
	}	
}
//----------------------------------------------------------------------
*/