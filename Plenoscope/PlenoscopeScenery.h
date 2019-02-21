// Copyright 2014 Sebastian A. Mueller
#ifndef PLENOSCOPE_PLENOSCOPESCENERY_H_
#define PLENOSCOPE_PLENOSCOPESCENERY_H_

#include <vector>
#include "Scenery/Scenery.h"
#include "Plenoscope/PlenoscopeInScenery.h"

namespace Plenoscope {

struct PlenoscopeInScenery;

struct PlenoscopeScenery: relleums::Scenery {
	std::vector<PlenoscopeInScenery> plenoscopes;
};

}  // namespace Plenoscope

#endif  // PLENOSCOPE_PLENOSCOPESCENERY_H_
