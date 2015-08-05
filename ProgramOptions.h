//=================================
// include guard
#ifndef __PROGRAMOPTIONS_H_INCLUDED__
#define __PROGRAMOPTIONS_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include "TracerException.h"
#include "Tools/Tools.h"
#include "Tools/FileTools.h"
#include "Core/TimeStamp.h"
#include "Tools/FiniteStateMashine.h"
#include "cmdline/cmdline.h"

//=================================
class KeyValueMap {
	std::map<std::string, std::string> options;
public:
	KeyValueMap();
	KeyValueMap(const std::string filename);
	void insert_key_and_value(const std::string key, const std::string value);
	bool has_key(const std::string)const;
	std::string get_value_for_key(const std::string key)const;
	bool get_value_for_key_as_bool(const std::string key)const;
	double get_value_for_key_as_double(const std::string key)const;
	int get_value_for_key_as_int(const std::string key)const;
	std::string get_print()const;
	void save(const std::string filename)const;
	void load(const std::string filename);
private:
	void parse_line(const std::string line);
	std::string get_file_print()const;
	void assert_key_is_unique(const std::string key)const;
	void assert_has_key(const std::string key)const;
	friend std::ostream& operator<<(std::ostream& os, const KeyValueMap& map);
	void assert_every_key_has_a_value(const int argc)const;
};
#endif // __PROGRAMOPTIONS_H_INCLUDED__ 