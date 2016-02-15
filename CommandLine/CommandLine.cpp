#include "CommandLine.h"

void CommandLine::add_value(
	const string name,
	const char short_name,
	const string desc
) {
	bool is_mandatory = false;
	cp.add<string>(name, short_name, desc, is_mandatory);
}

void CommandLine::add_flag(const string name, const string desc) {
	cp.add(name, '\0', desc);
}

void CommandLine::parse(int argc, char *argv[]) {
	cp.parse_check(argc, argv);
}

string CommandLine::get(const string key)const {
	return cp.get<string>(key);
}

bool CommandLine::exist(const string key)const {
	return cp.exist(key);
}