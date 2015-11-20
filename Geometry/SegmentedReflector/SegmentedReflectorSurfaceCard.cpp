#include "SegmentedReflector.h"
#include "Core/Function/ConstantFunction.h"
namespace SegmentedReflector {

	SurfaceCard::SurfaceCard(): 
		mirror_color(&Color::white),
		inner_mirror_color(&Color::dark_gray),
		outer_mirror_reflection(&Function::Constant::void_function)
	{}
} // SegmentedReflector