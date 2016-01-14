#include "HexagonalPrismZ.h"
//------------------------------------------------------------------------------
void HexagonalPrismZ::set_outer_radius(const double outer_radius) {

	//            /|\ Y-axis                                                  //
	//             |                                                          //
	//          ___|___                                                       //
	//  X-axis /   |   \                                                      //
	//  /_____/____|    \                                                     //
	//  \     \         /                                                     //
	//         \_______/                                                      //

	assert_outer_radius_positive(outer_radius);

	ex = Vector3D::unit_x;
	ey = Vector3D::unit_y;
	
	hexU = ey;
	
	double Pi_times_2over3 = 2.0/3.0*M_PI;

	hexV = ex*+sin( Pi_times_2over3 ) + ey*cos( Pi_times_2over3 );
	hexW = ex*-sin( Pi_times_2over3 ) + ey*cos( Pi_times_2over3 );
	
	inner_radius = outer_radius * cos( M_PI/6.0 );
	this->outer_radius = outer_radius;
}
//------------------------------------------------------------------------------
bool HexagonalPrismZ::is_inside(const Vector3D* vec)const {

	double projection_onto_hexU = hexU * *vec;
	double projection_onto_hexV = hexV * *vec;
	double projection_onto_hexW = hexW * *vec;

	return (
		projection_onto_hexU < inner_radius
		&&
		projection_onto_hexU > -inner_radius
		&&
		projection_onto_hexV < inner_radius
		&&
		projection_onto_hexV > -inner_radius
		&&
		projection_onto_hexW < inner_radius
		&&
		projection_onto_hexW > -inner_radius
	);
}
//------------------------------------------------------------------------------
void HexagonalPrismZ::assert_outer_radius_positive(const double outer_radius)const {

	if( outer_radius <= 0.0 ) {
		std::stringstream info;
		info << "HexagonalPrismZ::" << __func__ << "()\n";
		info << "The outer radius must not be smaller or equal zero! ";
		info << "Expected: outer_radius > 0m, but actual: outer radius = ";
		info << outer_radius  << "m\n";
		throw TracerException(info.str());
	}
}
//------------------------------------------------------------------------------
double HexagonalPrismZ::get_area()const{
	return outer_radius*outer_radius * 3.0/2.0 * sqrt(3.0);
}
//------------------------------------------------------------------------------
double HexagonalPrismZ::get_outer_radius()const {
	return outer_radius;
}