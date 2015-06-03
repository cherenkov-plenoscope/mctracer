#include "ProgramOptions.h"
//------------------------------------------------------------------------------
void KeyValueMap::insert_key_and_value(
	const std::string key, 
	const std::string value
) {
	assert_key_is_unique(key);
	options.insert(std::pair<std::string, std::string>(key, value));
}
//------------------------------------------------------------------------------
bool KeyValueMap::has_key(const std::string key)const {
	return options.find(key) != options.end();
}
//------------------------------------------------------------------------------
std::string KeyValueMap::get_value_for_key(const std::string key)const {
	assert_has_key(key);
	return options.find(key)->second;
}
//------------------------------------------------------------------------------
void KeyValueMap::assert_has_key(const std::string key)const {
	if(!has_key(key)) {
		std::stringstream out;
		out << "KeyValueMap has no key: '" << key << "'.";
		throw TracerException(out.str());
	}
}
//------------------------------------------------------------------------------
void KeyValueMap::assert_key_is_unique(const std::string key)const {
	if(has_key(key)) {
		std::stringstream out;
		out << "KeyValueMap has already a key: '" << key << "' with value: '";
		out << get_value_for_key(key) << "'";
		throw TracerException(out.str());
	}
}
//------------------------------------------------------------------------------
std::string KeyValueMap::get_print()const {

	std::stringstream out;
	int i = 0;
	out << "KeyValueMap: " << options.size() << " entries\n";
	for (auto& x: options) {
		i++;
		out << i << " key: '" << x.first << "', value: '" << x.second << "'\n";
	}
	return out.str();
}
//------------------------------------------------------------------------------
std::string KeyValueMap::get_file_print()const {

	TimeStamp now;

	std::stringstream out;
	int i = 0;
	out << "# written on: " << now.yyyy() << "." << now.mm() << "." << now.dd();
	out << " " << now.HH() << ":" << now.MM() << ":" << now.SS();
	out << " in yyyy.mm.dd HH:MM:SS UTC\n";
	for (auto& x: options) {
		i++;
		out << x.first << ": " << x.second << "\n";
	}
	return out.str();
}
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const KeyValueMap& map) {
    os << map.get_print();
    return os;
}
//------------------------------------------------------------------------------
KeyValueMap::KeyValueMap(const int argc, const char** argv) {

	if(argc > 0)
		insert_key_and_value("call", argv[0]);

	assert_every_key_has_a_value(argc);

    for (int i = 1; i<argc; i=i+2)
    	insert_key_and_value(argv[i], argv[i+1]);
}
//------------------------------------------------------------------------------
void KeyValueMap::assert_every_key_has_a_value(const int argc)const {
	if(is_even(argc))
		throw TracerException("argument count is even");
}
//------------------------------------------------------------------------------
KeyValueMap::KeyValueMap() {

}
//------------------------------------------------------------------------------
bool KeyValueMap::get_value_for_key_as_bool(const std::string key)const {
	return StrToBool(get_value_for_key(key));
}
//------------------------------------------------------------------------------
double KeyValueMap::get_value_for_key_as_double(const std::string key)const {
	return StrToDouble(get_value_for_key(key));
}
//------------------------------------------------------------------------------
int KeyValueMap::get_value_for_key_as_int(const std::string key)const {
	return StrToInt(get_value_for_key(key));
}
//------------------------------------------------------------------------------
void KeyValueMap::save(const std::string filename)const {
	FileTools::write_text_to_file(get_file_print(), filename);
}
//------------------------------------------------------------------------------
void KeyValueMap::load(const std::string filename)const {

	if(!FileTools::can_be_opened(filename)) {
		std::stringstream out;
		out << "KeyValueMap: Can not open '" << filename << "'";
		throw TracerException(out.str());
	}
}