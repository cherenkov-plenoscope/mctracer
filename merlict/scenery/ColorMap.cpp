// Copyright 2014 Sebastian A. Mueller
#include "merlict/scenery/ColorMap.h"
#include <sstream>


namespace merlict {

bool ColorMap::has(const std::string key)const {
    return colors.find(key) != colors.end();
}

Color* ColorMap::get(const std::string key) {
    assert_has(key);
    return &colors.find(key)->second;
}

void ColorMap::add(const std::string key, const Color color) {
    assert_not_in_use_yet(key);
    colors.insert(std::pair<std::string, Color>(key, color));
}

void ColorMap::assert_has(const std::string key)const {
    if (!has(key)) {
        std::stringstream info;
        info << "ColorMap " << __FILE__ << ", " << __LINE__ << "\n";
        info << "There is no color called '" << key << "'. \n";
        throw NoSuchKey(info.str());
    }
}

void ColorMap::assert_not_in_use_yet(const std::string key) {
    if (has(key)) {
        std::stringstream info;
        info << "ColorMap " << __FILE__ << ", " << __LINE__ << "\n";
        info << "The key '" << key << "' is already in use.\n";
        throw KeyAlreadyInUse(info.str());
    }
}

}  // namespace merlict
