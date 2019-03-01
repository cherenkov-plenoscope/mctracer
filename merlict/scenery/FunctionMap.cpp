// Copyright 2014 Sebastian A. Mueller
#include "merlict/scenery/FunctionMap.h"
#include <sstream>


namespace merlict {

bool FunctionMap::has(const std::string key)const {
    return functions.find(key) != functions.end();
}

const function::Func1* FunctionMap::get(const std::string key)const {
    assert_has(key);
    return &functions.find(key)->second;
}

void FunctionMap::add(const std::string key, const function::Func1 f) {
    assert_not_in_use_yet(key);
    functions.insert(std::pair<std::string, function::Func1>(key, f));
}

void FunctionMap::assert_has(const std::string key)const {
    if (!has(key)) {
        std::stringstream info;
        info << "FunctionMap " << __FILE__ << ", " << __LINE__ << "\n";
        info << "There is no function called '" << key << "'. \n";
        throw NoSuchKey(info.str());
    }
}

void FunctionMap::assert_not_in_use_yet(const std::string key) {
    if (has(key)) {
        std::stringstream info;
        info << "FunctionMap " << __FILE__ << ", " << __LINE__ << "\n";
        info << "The key '" << key << "' is already in use.\n";
        throw KeyAlreadyInUse(info.str());
    }
}

}  // namespace merlict
