#include "QuadraticEquation.h"
//------------------------------------------------------------------------------
QuadraticEquation::QuadraticEquation(double p, double q){
	p_over_2  = 0.5*p;
	this -> q = q;
	try_to_solve_equation();
}
//------------------------------------------------------------------------------
void QuadraticEquation::try_to_solve_equation(){
	inner_part_of_squareroot = p_over_2 * p_over_2 - q;
	calculate_squareroot_if_possible();
}
//------------------------------------------------------------------------------
void QuadraticEquation::calculate_squareroot_if_possible(){
	if( has_valid_solutions() )
		squareroot = sqrt( inner_part_of_squareroot );
}
//------------------------------------------------------------------------------
bool QuadraticEquation::has_valid_solutions(){
	return ( inner_part_of_squareroot >= 0.0 )? true : false;
}
//------------------------------------------------------------------------------
double QuadraticEquation::minus_solution(){
	return -p_over_2 - squareroot;
}
//------------------------------------------------------------------------------
double QuadraticEquation::plus_solution(){
	return -p_over_2 + squareroot;
}
//------------------------------------------------------------------------------