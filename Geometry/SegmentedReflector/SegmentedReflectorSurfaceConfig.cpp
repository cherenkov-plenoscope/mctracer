#include "SegmentedReflector.h"
#include "Core/Function/ConstantFunction.h"
namespace SegmentedReflector {

	SurfaceConfig::SurfaceConfig(): 
		mirror_color(&Color::white),
		inner_mirror_color(&Color::dark_gray),
		reflectivity(&Function::Constant::void_function)
	{}
} // SegmentedReflector