//=================================
// include guard
#ifndef __CORSIKAREADER_H_INCLUDE__
#define __CORSIKAREADER_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies  
#include "Corsika/EventIo.h"
#include "Corsika/CorsikaPhotonFactory.h"

class CorsikaReader: public printable {

	bool is_mmcs;
	bool is_event_io;
public:
	
	CorsikaReader(const std::string path);
};

#endif // __CORSIKAREADER_H_INCLUDE__