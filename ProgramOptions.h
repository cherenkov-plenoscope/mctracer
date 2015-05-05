//=================================
// include guard
#ifndef __PROGRAMOPTIONS_H_INCLUDED__
#define __PROGRAMOPTIONS_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include "TracerException.h"

//=================================
class KeyValueMap {
	std::map<std::string, std::string> options;
public:
	KeyValueMap();
	KeyValueMap(const int argc, const char** argv);
	void insert_key_and_value(const std::string key, const std::string value);
	bool has_key(const std::string)const;
	std::string get_value_for_key(const std::string key)const;
	std::string get_print()const;
private:
	void assert_key_is_unique(const std::string key)const;
	void assert_has_key(const std::string key)const;
	friend std::ostream& operator<<(std::ostream& os, const KeyValueMap& map);
};

#endif // __PROGRAMOPTIONS_H_INCLUDED__ 
