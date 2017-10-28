//=================================
// include guard
#ifndef __COLORMAP_H_INCLUDED__
#define __COLORMAP_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <string>
#include <map>
#include "ResourceMap.h"
#include "Core/Color.h"

class ColorMap: public ResourceMap {
public:
    std::map<std::string, Color> colors;

    bool has(const std::string key)const;
    Color* get(const std::string key);
    void add(const std::string key, const Color color);
private:

    void assert_has(const std::string key)const;
    void assert_not_in_use_yet(const std::string key);
};
#endif // __COLORMAP_H_INCLUDED__