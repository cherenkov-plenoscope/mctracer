// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_COLORMAP_H_
#define SCENERY_COLORMAP_H_

#include <string>
#include <map>
#include "ResourceMap.h"
#include "Core/Color.h"

struct ColorMap: public ResourceMap {
    std::map<std::string, Color> colors;
    bool has(const std::string key)const;
    Color* get(const std::string key);
    void add(const std::string key, const Color color);
    void assert_has(const std::string key)const;
    void assert_not_in_use_yet(const std::string key);
};

#endif  // SCENERY_COLORMAP_H_
