//=================================
// include guard
#ifndef __QUADRATIHEQUATION_H_INCLUDED__
#define __QUADRATIHEQUATION_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
  
class QuadraticEquation{
	// f(x) = ax^2 + bx + c

	// p = b/a
	// q = c/a

	// x_+ = -p/2 + sqrt( (p/2)^2 - q )

	double p_over_2;
	double q;

	double inner_part_of_squareroot;
	double squareroot;
public:
    QuadraticEquation(const double _p, const double _q);
	bool has_valid_solutions();
	double minus_solution();
	double plus_solution();
};

#endif // __QUADRATIHEQUATION_H_INCLUDED__