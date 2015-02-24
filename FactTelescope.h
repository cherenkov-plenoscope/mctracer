//=================================
// include guard
#ifndef __FACTTELESCOPE_H_INCLUDED__
#define __FACTTELESCOPE_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream> 
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include "CartesianFrame.h"
#include "SphereCapWithHexagonalBound.h"
#include "SurfaceProperties.h"

//=================================
class FactTelescope :public CartesianFrame{
// the fact telescope class stores the geometric information of fact
// and creates a 3D telescope reflector in the world
	std::stringstream telescope_table;

// the inner radius R1 reflector part
	double focal_length_telescope;
	double MirrorRadius_m;
	std::vector<SphereCapWithHexagonalBound*> list_of_mirrors;

	SurfaceProperties* outer_mirror_surface;
	SurfaceProperties* inner_mirror_surface;
public:

	FactTelescope(double alpha);
	
	void disp();
	
	std::string get_fact_string();

	void export_table_of_telescope(std::string telescope_table_file_name);
	
	~FactTelescope();
	void init();
private:
	void assert_alpha_geometry_parameter_is_valid(const double alpha)const;
	void init_outer_mirror_surface();
	void init_inner_mirror_surface();
};
#endif // __FACTTELESCOPE_H_INCLUDED__ 
