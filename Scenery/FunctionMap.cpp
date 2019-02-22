// Copyright 2014 Sebastian A. Mueller
#include "Scenery/FunctionMap.h"
#include <sstream>
using std::string;
using std::map;
using std::stringstream;

namespace relleums {

bool FunctionMap::has(const string key)const {
    return functions.find(key) != functions.end();
}

Function::Func1* FunctionMap::get(const string key)const {
    assert_has(key);
    return functions.find(key)->second;
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

FunctionMap::~FunctionMap() {
    for (std::pair<string, Function::Func1*> function : functions)
        delete function.second;
}

}  // namespace relleums
