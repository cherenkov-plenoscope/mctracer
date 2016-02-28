//=================================
// include guard
#ifndef __CommandLine_H_INCLUDED__
#define __CommandLine_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "cmdline/cmdline.h"
#include <string>
// A command line wrapper

using std::string;
//------------------------------------------------------------------------------
namespace CommandLine {

	class Parser {
	 	
		cmdline::parser cp;	
	public:

		void define_key_val_by_key_short_desc(const string name, const char short_name, const string desc);
		void define_command(const string name, const string desc);
		void parse(int argc, char *argv[]);
		string get(const string key)const;
		bool exist(const string key)const;
	};
}//CommandLine
#endif // __CommandLine_H_INCLUDED__ 