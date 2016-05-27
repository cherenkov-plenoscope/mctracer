#include "Plenoscope/VisualInspection.h"

namespace Plenoscope {
//------------------------------------------------------------------------------
VisualInspection::VisualInspection(const Factory *factory): fab(factory) {
	
}
//------------------------------------------------------------------------------
void VisualInspection::set_up_world_basics() {
	world.set_name_pos_rot("inspection_world", Vec3::null, Rot3::null);

	ground.set_name_pos_rot(
		"ground", 
		Vec3(0.0, 0.0, -fab->geometry->reflector.max_outer_aperture_radius()), 
		Rot3::null
	);

	ground.set_radius(fab->geometry->reflector.max_outer_aperture_radius()*100.0);
	ground.set_outer_color(&Color::grass_green);
	ground.set_inner_color(&Color::grass_green);

	world.set_mother_and_child(&ground);
}
//------------------------------------------------------------------------------
}