// Copyright 2014 Sebastian A. Mueller
#ifndef PLENOSCOPE_PLENOSCOPESCENERY_H_
#define PLENOSCOPE_PLENOSCOPESCENERY_H_

#include <vector>
#include "merlict/merlict.h"
#include "merlict_portal_plenoscope/PlenoscopeInScenery.h"

namespace plenoscope {

struct PlenoscopeInScenery;

struct PlenoscopeScenery: merlict::Scenery {
	std::vector<PlenoscopeInScenery> plenoscopes;
};

}  // namespace plenoscope

#endif  // PLENOSCOPE_PLENOSCOPESCENERY_H_
