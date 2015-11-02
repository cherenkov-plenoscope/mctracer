#include "KeyValueMap.h"
#include <sstream>
#include "TracerException.h"
#include "Tools/Tools.h"
#include "Tools/StringTools.h"
#include "Tools/FileTools.h"
#include "Core/TimeStamp.h"
namespace KeyValueMap {
//------------------------------------------------------------------------------
Map::Map() {}
//------------------------------------------------------------------------------
Map::Map(const std::string filename) {
	Io mapIO;
	(*this) = mapIO.load(filename);
}
//------------------------------------------------------------------------------
void Map::save(const std::string filename)const {
	Io mapIO;
	mapIO.save_map_to_file(*this, filename);
}
//------------------------------------------------------------------------------
std::string Map::get_filename()const {return filename;}
//------------------------------------------------------------------------------
void Map::insert_key_and_value(
	const std::string key, 
	const std::string value
) {
	assert_key_is_unique(key);
	options.insert(std::pair<std::string, std::string>(key, value));
}
//------------------------------------------------------------------------------
bool Map::has_key(const std::string key)const {
	return options.find(key) != options.end();
}
//------------------------------------------------------------------------------
std::string Map::get_value_for_key(const std::string key)const {
	assert_has_key(key);
	return options.find(key)->second;
}
//------------------------------------------------------------------------------
void Map::assert_has_key(const std::string key)const {
	if(!has_key(key)) {
		std::stringstream out;
		out << "KeyValueMap " << __FILE__ << ", " << __LINE__ << "\n";
		if(!filename.empty()) out << filename << "\n";
		out << "KeyValueMap has no key: '" << key << "'. \n";
		throw TracerException(out.str());
	}
}
//------------------------------------------------------------------------------
void Map::assert_key_is_unique(const std::string key)const {
	if(has_key(key)) {
		std::stringstream out;
		out << "KeyValueMap " << __FILE__ << ", " << __LINE__ << "\n";
		if(!filename.empty()) out << filename << "\n";
		out << "Has already a key: '" << key << "' with value: '";
		out << get_value_for_key(key) << "'\n";
		throw TracerException(out.str());
	}
}
//------------------------------------------------------------------------------
std::string Map::get_print()const {

	std::stringstream out;
	out << get_file_print();
	return out.str();
}
//------------------------------------------------------------------------------
std::string Map::get_file_print()const {

	std::stringstream out;
	for (auto& x: options)
		out << x.first << ": " << x.second << "\n";
	return out.str();
}
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const Map& map) {
    os << map.get_print();
    return os;
}
//------------------------------------------------------------------------------
void Map::assert_every_key_has_a_value(const int argc)const {
	if(is_even(argc))
		throw TracerException("Map: argument count is even");
}
//------------------------------------------------------------------------------
bool Map::get_value_for_key_as_bool(const std::string key)const {
	return StrToBool(get_value_for_key(key));
}
//------------------------------------------------------------------------------
double Map::get_value_for_key_as_double(const std::string key)const {
	return StrToDouble(get_value_for_key(key));
}
//------------------------------------------------------------------------------
int Map::get_value_for_key_as_int(const std::string key)const {
	return StrToInt(get_value_for_key(key));
}
//------------------------------------------------------------------------------
// IO
//------------------------------------------------------------------------------
void Io::save_map_to_file(
	const Map map, const std::string filename
) {
	FileTools::write_text_to_file(map.get_file_print(), filename);	
}
//------------------------------------------------------------------------------
Map Io::load(const std::string filename) {

	map.filename = filename;
	std::ifstream myfile (map.filename.c_str());
	
	if(myfile.is_open())
		parse_and_close(myfile);
	else 
		throw_can_not_open();

	return map;
}
//------------------------------------------------------------------------------
void Io::parse_and_close(std::ifstream &myfile) {
	
	line_number = 0;
	std::string line;
	while(getline (myfile,line))
		parse_line(line);
	myfile.close();
}
//------------------------------------------------------------------------------
void Io::throw_can_not_open()const {
	std::stringstream out;
	out << "KeyValueMap::IO " << __FILE__ << ", " << __LINE__ << "\n";
	out << "Can not open '" << map.filename << "'";
	throw TracerException(out.str());
}
//------------------------------------------------------------------------------
void Io::parse_line(const std::string line) {

	line_number++;
	std::string key, value;

	bool is_comment = false;
	bool colon_was_found = false;

	uint i=0;
	while(i<line.length()) {
		if(line[i] == '#') {
			is_comment = true;
			break;
		}

		if(line[i] == ':') {
			colon_was_found = true;
			if(i+1<line.length()) i++; else break;
		}

		if(colon_was_found)
			value.push_back(line[i]);
		else
			key.push_back(line[i]);
		i++;
	}

	if(!colon_was_found && !is_comment) {
		
		std::stringstream out;
		out << "KeyValueMap::IO " << __FILE__ << ", " << __LINE__ << "\n";
		out << "Can not parse line " << line_number << " in file '";
		out << map.filename << "'\n";
		out << "line: '" << line << "\n";
		throw TracerException(out.str());
	}

	if(!is_comment) {
		key = StringTools::strip_whitespaces(key);
		value = StringTools::strip_whitespaces(value);	
		map.insert_key_and_value(key, value);	
	}
}
} // KeyValueMap