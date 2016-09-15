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

using std::string;
using std::map;

#include "ResourceMap.h"
#include "Core/Color.h"

class ColorMap: public ResourceMap {
public:
    map<string, Color> colors;

    bool has(const string key)const;
    const Color* get(const string key)const;
    void add(const string key, const Color color);
private:

    void assert_has(const string key)const;
    void assert_not_in_use_yet(const string key);
};
#endif // __COLORMAP_H_INCLUDED__