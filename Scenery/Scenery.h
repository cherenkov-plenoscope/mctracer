//=================================
// include guard
#ifndef __SCENERY_H_INCLUDED__
#define __SCENERY_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using std::string;
using std::stringstream;
using std::vector;

#include "ColorMap.h"
#include "FunctionMap.h"
#include "Core/Frame.h"

struct Scenery {

    Frame root;

    ColorMap colors;
    FunctionMap functions;

    Scenery();

    static Scenery void_scenery;
};
#endif // __SCENERY_H_INCLUDED__