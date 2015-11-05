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
#include <map>
#include "Core/Printable.h"

//=================================
namespace KeyValueMap{

	class Io;
	
	class Map  : public Printable{

		std::map<std::string, std::string> options;
		std::string filename = "";
	public:

		Map();
		Map(const std::string _filename);
		void insert_key_and_value(const std::string key, const std::string value);
		bool has_key(const std::string)const;
		std::string get_value_for_key(const std::string key)const;
		bool get_value_for_key_as_bool(const std::string key)const;
		double get_value_for_key_as_double(const std::string key)const;
		int get_value_for_key_as_int(const std::string key)const;
		std::string get_print()const;
		std::string get_file_print()const;
		void save(const std::string _filename)const;
		std::string get_filename()const;
	private:

		void assert_key_is_unique(const std::string key)const;
		void assert_has_key(const std::string key)const;
		void assert_every_key_has_a_value(const int argc)const;
		friend KeyValueMap::Io;
	};	

	class Io {
	public:

		void save_map_to_file(const Map map, const std::string filename);
		Map load(const std::string filename);
	private:

		Map map;
		std::string filename;
		uint line_number;
		void throw_can_not_open()const;
		void parse_and_close(std::ifstream &myfile);
		void parse_line(const std::string line);
	};
}




#endif // __PROGRAMOPTIONS_H_INCLUDED__ 