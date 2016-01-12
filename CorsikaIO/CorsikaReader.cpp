#include "CorsikaIO/CorsikaReader.h"

//------------------------------------------------------------------------------
CorsikaReader::CorsikaReader(const std::string path) {

	// Try to open as MAGIC Corsica aka. MMCS
	try{
		MmcsCorsikaFullEventGetter event_getter(path);
		is_mmcs = true;
	}catch(std::exception &error) {
		is_mmcs = false;
	}

	if(is_mmcs) return;

	// Try to open as Mr. 'this needs to run on a VAX computer as well' Bernloeh 
	// Event IO
	try{
		EventIo::EventIoFile corsika_run(path);
		is_eventio = true;
	}catch(std::exception &error) {
		is_eventio = false;
	}

	if(is_eventio) return;

	throw TracerException("neither EventIO nor MMCS");
}
