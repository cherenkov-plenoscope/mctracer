#include "Geometry/SegmentedReflector/Config.h"
#include "KeyValueMap.h"
#include "Tools/AsciiIo.h"

namespace SegmentedReflector {

	Config::Config(): 
		// Default is HEGRA CT3 aka FACT
		focal_length(4.889),
		DaviesCotton_over_parabolic_mixing_factor(0.5),
		max_outer_aperture_radius(2.1),
		min_inner_aperture_radius(0.2),
		facet_inner_hex_radius(0.30),
		gap_between_facets(0.01),

		mirror_color(&Color::white),
		inner_mirror_color(&Color::dark_gray),
		reflectivity(&perfect_reflectivity)
	{}

	Config load_config_from_file(const std::string path) {

		KeyValueMap::Map config_map(path);

		Config config;

		config.focal_length = 
			config_map.get_value_for_key_as_double("focal_length");

		config.DaviesCotton_over_parabolic_mixing_factor = 
			config_map.get_value_for_key_as_double("DaviesCotton_over_parabolic_mixing_factor");

		config.max_outer_aperture_radius = 
			config_map.get_value_for_key_as_double("max_outer_aperture_radius");

		config.min_inner_aperture_radius = 
			config_map.get_value_for_key_as_double("min_inner_aperture_radius");

		config.facet_inner_hex_radius = 
			config_map.get_value_for_key_as_double("facet_inner_hex_radius");

		config.gap_between_facets = 
			config_map.get_value_for_key_as_double("gap_between_facets");	

		config.mirror_color = new Color(
			config_map.get_value_for_key_as_int("mirror_color_r"),
			config_map.get_value_for_key_as_int("mirror_color_g"),
			config_map.get_value_for_key_as_int("mirror_color_b")
		);

		config.inner_mirror_color = new Color(
			config_map.get_value_for_key_as_int("inner_mirror_color_r"),
			config_map.get_value_for_key_as_int("inner_mirror_color_g"),
			config_map.get_value_for_key_as_int("inner_mirror_color_b")
		);

		config.reflectivity = new Function::LinInterpol(
			AsciiIo::gen_table_from_file(
				config_map.get_value_for_key("reflector_mirror_reflectivity")
			)
		);

		return config;
	}

	void save_config_to_file(Config config, const std::string path) {

	}
} // SegmentedReflector