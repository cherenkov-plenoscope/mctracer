// Copyright 2014 Sebastian A. Mueller
#include "Core/scenery/FunctionMap.h"
#include <sstream>
using std::string;
using std::map;
using std::stringstream;

namespace relleums {

bool FunctionMap::has(const string key)const {
    return functions.find(key) != functions.end();
}

const function::Func1* FunctionMap::get(const string key)const {
    assert_has(key);
    return &functions.find(key)->second;
}

void FunctionMap::add(const std::string key, const function::Func1 f) {
    assert_not_in_use_yet(key);
    functions.insert(std::pair<std::string, function::Func1>(key, f));
}

void FunctionMap::assert_has(const string key)const {
    if (!has(key)) {
        stringstream info;
        info << "FunctionMap " << __FILE__ << ", " << __LINE__ << "\n";
        info << "There is no function called '" << key << "'. \n";
        throw NoSuchKey(info.str());
    }
}

void FunctionMap::assert_not_in_use_yet(const string key) {
    if (has(key)) {
        stringstream info;
        info << "FunctionMap " << __FILE__ << ", " << __LINE__ << "\n";
        info << "The key '" << key << "' is already in use.\n";
        throw KeyAlreadyInUse(info.str());
    }
}

}  // namespace relleums
