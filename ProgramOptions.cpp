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
	out << "KeyValueMap: " << options.size() << " entries\n";
	for (auto& x: options) {
		out << "key: '" << x.first << "', value: '" << x.second << "'\n";
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
    std::cout << "Have " << argc << " arguments:" << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << std::endl;
    }	
}
//------------------------------------------------------------------------------
KeyValueMap::KeyValueMap() {

}