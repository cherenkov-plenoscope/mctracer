//=================================
// include guard
#ifndef __SegmentedReflectorConfig_H_INCLUDED__
#define __SegmentedReflectorConfig_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Tools/Tools.h"
#include "Core/SurfaceEntity.h"
#include "HexGridXy.h"

//using Function::Func1D;

class SegmentedReflectorConfig {

	// main reflector
	double focal_length;

	double DaviesCotton_over_parabolic_mixing_factor;
	double davies_cotton_weight;
	double parabolic_weight;

	double max_outer_aperture_radius;
	double min_inner_aperture_radius;

	double facet_inner_hex_radius;
	double facet_cell_overhead;

	//const Function::Func1D* mirror_facet_reflecivity_vs_wavelength = &Function::Constant::void_function;	
	//const Color *mirror_colour = &Color::white;
	//const Color *inner_mirror_colour = &Color::dark_gray;

	// internal
	std::vector<Vector3D> facet_xy_positions;
	Vector3D focal_point;
public:	
	SegmentedReflectorConfig() {
		focal_length = 78.0;

		DaviesCotton_over_parabolic_mixing_factor = 0.0;

		max_outer_aperture_radius = 26.2;
		min_inner_aperture_radius = 0.0;

		facet_inner_hex_radius = 0.3;
		facet_cell_overhead = 1.02;	

		init();	
	}
	//------------------------------------------------
	// SET
	//------------------------------------------------
	void set_focal_length(const double f) {
		focal_length = f;
		init();
	};

	void set_max_outer_aperture_radius(const double Ro) {
		max_outer_aperture_radius = Ro;
		init();
	};

	void set_min_inner_aperture_radius(const double Ri) {
		min_inner_aperture_radius = Ri;
		init();
	};

	void set_DaviesCotton_over_Parabolic_ratio(const double dc_over_para) {
		DaviesCotton_over_parabolic_mixing_factor = dc_over_para;
		init();
	};

	void set_inner_facet_radius(const double fihr) {
		facet_inner_hex_radius = fihr;
		init();
	};
	//------------------------------------------------
	// INIT
	//------------------------------------------------
	void init() {
		init_xy_grid();
		init_focal_point();
	};

	void init_xy_grid() {

		if(min_inner_aperture_radius == 0.0) {

			HexGridXy hex_grid(
				max_outer_aperture_radius - get_facet_spacing()/2.0,
				get_facet_spacing()

			);

			facet_xy_positions = hex_grid.get_grid();
		}else{

			HexGridXy hex_grid(
				max_outer_aperture_radius - get_facet_spacing()/2.0,
				min_inner_aperture_radius + get_facet_spacing()/2.0,
				get_facet_spacing()
			);

			facet_xy_positions = hex_grid.get_grid();
		}
	};

	void init_focal_point() {
		focal_point = Vector3D(0.0, 0.0, focal_length);
	}
	
	//------------------------------------------------
	// GET
	//------------------------------------------------
	double get_focal_length()const {
		return focal_length;
	};

	Vector3D get_focal_point()const {
		return focal_point;
	};

	double get_DaviesCotton_over_parabolic_mixing_factor()const {
		return DaviesCotton_over_parabolic_mixing_factor;
	};

	double get_DaviesCotton_weight()const {
		return DaviesCotton_over_parabolic_mixing_factor;
	};

	double get_Parabolic_weight()const {
		return 1.0 - DaviesCotton_over_parabolic_mixing_factor;
	};

	double get_max_outer_aperture_radius()const {
		return max_outer_aperture_radius;
	};

	double get_min_inner_aperture_radius()const {
		return min_inner_aperture_radius;
	};

	double get_facet_inner_hex_radius()const {
		return facet_inner_hex_radius;
	}

	double get_facet_spacing()const {
		return facet_inner_hex_radius*2.0*facet_cell_overhead;
	};

	double get_gap_between_facet_edged()const {
		return (get_facet_spacing() - 2.0*get_facet_inner_hex_radius());
	};

	double get_naive_Fnumber()const {
		return get_focal_length()/(2.0*get_max_outer_aperture_radius());
	};

	double get_Fnumber()const {
		double radius_of_single_mirror_with_same_area = 
			sqrt(get_effective_area()/M_PI);

		return get_focal_length()/(radius_of_single_mirror_with_same_area*2.0);
	};

	double get_naive_area()const {
		double Ro = get_max_outer_aperture_radius();
		double Ri = get_min_inner_aperture_radius();
		return M_PI*(Ro*Ro - Ri*Ri);
	};

	double get_effective_area()const {
		return get_number_of_facets()*get_facet_area();
	};

	double get_facet_area()const {
		return get_facet_inner_hex_radius()*get_facet_inner_hex_radius()*
			6.0/sqrt(3.0); 
	};

	double get_number_of_facets()const {
		return facet_xy_positions.size();
	};

	std::string get_print()const {

		std::stringstream out;
		//out.setf(std::ios_base::fixed, std::ios_base::floatfield);
		out << std::setprecision(4);
		out << "__Segmented_Reflector__\n";
		out << " focal length................... " << get_focal_length() << "m\n";
		out << " focal point.................... " << get_focal_point() << "\n";
		out << " max outer aperture diameter.... " << 2.0*get_max_outer_aperture_radius() << "m\n";
		out << " min inner aperture diameter.... " << 2.0*get_min_inner_aperture_radius() << "m\n";
		out << " naive F-Number................. " << get_naive_Fnumber() << "\n";
		out << " effective F-Number............. " << get_Fnumber() << "\n";
		out << " naive area..................... " << get_naive_area() << "m^2\n";
		out << " effective area................. " << get_effective_area() << "m^2\n";
		out << " number of facets............... " << get_number_of_facets() << "\n";
		out << " facet area..................... " << get_facet_area() << "m^2\n";
		out << " facet inner radius............. " << get_facet_inner_hex_radius() << "m\n";
		out << " facet spacing.................. " << get_facet_spacing() << "m\n";
		out << " gap between facets............. " << get_gap_between_facet_edged()*1e3 << "mm\n";
		out << " Davies Cotton weight........... " << get_DaviesCotton_weight() << "\n";
		out << " Parabolic weight............... " << get_Parabolic_weight() << "\n";
	return out.str();		
	}
};
#endif // __SegmentedReflectorConfig_H_INCLUDED__ 
