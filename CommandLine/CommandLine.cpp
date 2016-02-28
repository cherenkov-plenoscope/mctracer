#include "CommandLine.h"
namespace CommandLine {
	
	void Parser::define_key_val_by_key_short_desc(
		const string name,
		const char short_name,
		const string desc
	) {
		bool is_mandatory = false;
		cp.add<string>(name, short_name, desc, is_mandatory);
	}

	void Parser::define_command(const string name, const string desc) {
		cp.add(name, '\0', desc);
	}

	void Parser::parse(int argc, char *argv[]) {
		cp.parse_check(argc, argv);
	}

	string Parser::get(const string key)const {
		return cp.get<string>(key);
	}

	bool Parser::exist(const string key)const {
		return cp.exist(key);
	}
}//CommandLine