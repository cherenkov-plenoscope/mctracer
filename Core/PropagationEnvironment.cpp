#include "PropagationEnvironment.h"
//------------------------------------------------------------------------------
void PropagationEnvironment::assert_completeness()const {
	if(world_geometry == nullptr)
		throw_missing_item("Frame world_geometry");

	if(propagation_options == nullptr)
		throw_missing_item("Propagation options");

	if(random_engine == nullptr)
		throw_missing_item("Pseudo random number generator");
}
//------------------------------------------------------------------------------
void PropagationEnvironment::throw_missing_item(
	const std::string name_of_missing_item
)const {
	std::stringstream info;
	info << "PropagationEnvironment is missing an item.\n";
	info << name_of_missing_item << " has not been set!";
	throw TracerException(info.str());
}
//------------------------------------------------------------------------------