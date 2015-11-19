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

class SegmentedReflectorConfig :public Printable {

	double focal_length;

	double DaviesCotton_over_parabolic_mixing_factor;
	double davies_cotton_weight;
	double parabolic_weight;

	double max_outer_aperture_radius;
	double min_inner_aperture_radius;

	double facet_inner_hex_radius;
	double gap_between_facets;

	double z_offset_makeing_avg_facet_dist_to_f_point_match_f;

	std::vector<Vector3D> facet_positions;
	Vector3D focal_point;
public:	
	SegmentedReflectorConfig() {
		focal_length = 75;

		DaviesCotton_over_parabolic_mixing_factor = 0.5;

		max_outer_aperture_radius = 25.75;
		min_inner_aperture_radius = 0.0;

		facet_inner_hex_radius = 0.6;
		gap_between_facets = 0.01;	

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

	void set_gap_between_facets(const double gap) {
		gap_between_facets = gap;
		init();
	};
	//------------------------------------------------
	// INIT
	//------------------------------------------------
	void init() {
		init_focal_point();
		init_facet_xy_positions();
		init_facet_z_positions();
		make_average_facet_distance_to_focal_point_match_focal_length();
	};

	void init_focal_point() {
		focal_point = Vector3D(0.0, 0.0, focal_length);
	};

	void init_facet_xy_positions() {

		HexGridXy hex_grid(
			max_outer_aperture_radius - get_facet_spacing()/2.0,
			min_inner_aperture_radius + get_facet_spacing()/2.0,
			get_facet_spacing()
		);

		facet_positions = hex_grid.get_grid();
	};

	void init_facet_z_positions() {

		for(uint i=0; i<facet_positions.size(); i++) {

			facet_positions.at(i).set(
				facet_positions.at(i).x(),
				facet_positions.at(i).y(),
				get_z_pos_given_dist_to_optical_axis(
					facet_positions.at(i).get_projection_on_XY_plane().norm()
				)
			);
		}
	};

	void make_average_facet_distance_to_focal_point_match_focal_length() {

		z_offset_makeing_avg_facet_dist_to_f_point_match_f = 0.0;
		uint iteration_conter = 0;
		double deviation = 0.0;

		do{
			abort_if_too_many_iterations(++iteration_conter);

			deviation = focal_length - get_average_image_distances_of_facets();

			double step = -deviation*0.5;
			move_all_facets_in_z(step);
			z_offset_makeing_avg_facet_dist_to_f_point_match_f += step;

		}while( fabs(deviation) > focal_length*1e-6);
	}
	
	void abort_if_too_many_iterations(const uint iteration_conter) {
		
		if(iteration_conter > 100) {
			std::stringstream info;
			info << __FILE__ << ", " << __LINE__<< "\n";
			info << "Exceeded max number of 100 iterations. ";
			info << "Can not reach best reflector z position. ";
			throw TracerException(info.str());
		}
	}

	double get_average_image_distances_of_facets() {

		uint number_of_facets = 0;
		double sum_of_image_distances = 0.0;

		for(Vector3D facet_pos: facet_positions) {
			number_of_facets++;
			sum_of_image_distances = sum_of_image_distances + 
				(facet_pos - focal_point).norm();
		}

		return sum_of_image_distances/double(number_of_facets);
	}

	void move_all_facets_in_z(const double movement) {

		for(uint i=0; i<facet_positions.size(); i++) {

			facet_positions.at(i).set(
				facet_positions.at(i).x(),
				facet_positions.at(i).y(),
				facet_positions.at(i).z() + movement
			);
		}	
	}
	//------------------------------------------------
	// GET
	//------------------------------------------------
	double get_z_pos_given_dist_to_optical_axis(const double dist)const {

		const double zDC = get_z_Davies_Cotton_given_dist_to_optical_axis(dist);
		const double zPa = get_z_Parabola_given_dist_to_optical_axis(dist);
		const double DC_over_Pa = DaviesCotton_over_parabolic_mixing_factor;

		return DC_over_Pa*zDC + (1.0 - DC_over_Pa)*zPa;
	};

	double get_z_Davies_Cotton_given_dist_to_optical_axis(double dist)const {
		return focal_length - sqrt(focal_length*focal_length - dist*dist);
	};

	double get_z_Parabola_given_dist_to_optical_axis(double dist)const {
		return 1.0/(4.0*focal_length)*dist*dist;
	};

	double get_focal_length()const {
		return focal_length;
	};

	Vector3D get_focal_point()const {
		return focal_point;
	};

	std::vector<Vector3D> get_facet_positions()const {
		return facet_positions;
	}

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
		return facet_inner_hex_radius*2.0+gap_between_facets;
	};

	double get_gap_between_facet_edged()const {
		return gap_between_facets;
	};

	double get_naive_Fnumber()const {
		return get_focal_length()/(2.0*get_max_outer_aperture_radius());
	};

	double get_Fnumber()const {
		return get_focal_length()/(get_effective_radius()*2.0);
	};

	double get_naive_area()const {
		double Ro = get_max_outer_aperture_radius();
		double Ri = get_min_inner_aperture_radius();
		return M_PI*(Ro*Ro - Ri*Ri);
	};

	double get_effective_area()const {
		return get_number_of_facets()*get_facet_area();
	};

	double get_effective_radius()const {
		return sqrt(get_effective_area()/M_PI);
	};

	double get_facet_area()const {
		return get_facet_inner_hex_radius()*get_facet_inner_hex_radius()*
			6.0/sqrt(3.0); 
	};

	double get_number_of_facets()const {
		return facet_positions.size();
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
		out << " naive area..................... " << get_naive_area() << "m^2\n";
		out << " thickness of dish.............. " << get_z_pos_given_dist_to_optical_axis(get_max_outer_aperture_radius()) << "m\n";
		out << " z offset for best focus........ " << z_offset_makeing_avg_facet_dist_to_f_point_match_f << "m\n";
		out << " effective F-Number............. " << get_Fnumber() << "\n";
		out << " effective area................. " << get_effective_area() << "m^2\n";
		out << " effective diameter............. " << 2.0*get_effective_radius() << "m\n";
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
