#include "Plenoscope/LightFieldSensor/Config.h"
#include "Core/SurfaceEntity.h"

namespace Plenoscope {
	namespace LightFieldSensor {
		Config::Config() {
			expected_imaging_system_focal_length = 75.0;
			expected_imaging_system_max_aperture_radius = 25.0;
			max_FoV_diameter = Deg2Rad(6.5);
			pixel_FoV_hex_flat2flat = Deg2Rad(0.1);	
			number_of_paxel_on_pixel_diagonal = 13;
			housing_overhead = 1.2;
			lens_refraction = &pmma_refraction;
			lens_absorbtion = &perfect_transparency;
			bin_relection = SurfaceEntity::default_refl;
		}
	}
}