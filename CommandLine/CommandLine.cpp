#include "CommandLine.h"

void CommandLine::define_name_short_description(
	const string name,
	const char short_name,
	const string desc
) {
	bool is_mandatory = false;
	cp.add<string>(name, short_name, desc, is_mandatory);
}

void CommandLine::parse(int argc, char *argv[]) {
	cp.parse_check(argc, argv);
}

string CommandLine::get(const string key)const {
	return cp.get<string>(key);
}