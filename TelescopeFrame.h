//=================================
// include guard
#ifndef __TelescopeFrame_H_INCLUDED__
#define __TelescopeFrame_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "CartesianFrame.h"

class TelescopeFrame : public CartesianFrame {

	double Az_Rad;
	double Zd_Rad;
public:
	void move_to_Az_Zd(const double Az_Rad, const double Zd_Rad);
	std::string get_pointing_print()const;
};
#endif // __TelescopeFrame_H_INCLUDED__