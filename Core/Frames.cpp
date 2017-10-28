#include "Core/Frames.h"
#include "Scenery/Geometry/QuadraticEquation.h"
using std::string;
using std::stringstream;
using std::vector;

namespace Frames {
//------------------------------------------------------------------------------
Vec3 bounding_sphere_center(const vector<Frame*> &frames) {
	// this can be optimized using Fischer's thesis on spheres enclosing spheres

	Vec3 center = mean_of_positions_in_mother(frames);

	if(frames.size() < Frame::max_number_of_children) {
		const Vec3 alternative = dumb_bounding_sphere_center(frames);
		const double m_radius = bounding_sphere_radius(frames, center); 
		const double alt_radius = bounding_sphere_radius(frames, alternative);
		if(m_radius > alt_radius)
			center = alternative;
	}
	return center;
}
//------------------------------------------------------------------------------
Vec3 dumb_bounding_sphere_center(const vector<Frame*> &frames) {

	if(frames.size() < 1) {
		stringstream info;
		info << __FILE__ << ", " << __LINE__ << "\n";
		info << "Expected at least 1 frame to calculate center position of frames, ";
		info << "but actual there are only " << frames.size() << " frames.";
		throw std::invalid_argument(info.str());		
	}

	if(frames.size() == 1)
		return frames.at(0)->get_position_in_mother();

	Frame* start_frame = &Frame::void_frame;
	Frame* end_frame = &Frame::void_frame;
   	double maximum = 0.0;
	for(uint i=0; i<frames.size(); i++) {
		for(uint j=i; j<frames.size(); j++) {
			if(i != j) {
				double dist = (
						frames[i]->get_position_in_mother() - 
						frames[j]->get_position_in_mother()
					).norm() + 
					frames[i]->get_bounding_sphere_radius() + 
					frames[j]->get_bounding_sphere_radius();

				if(dist > maximum) {
					maximum = dist;
					start_frame = frames[i];
					end_frame =  frames[j];
				}
			}
		}
	}

	Vec3 dir = end_frame->get_position_in_mother() - 
		start_frame->get_position_in_mother();

	dir = dir/dir.norm();
	return start_frame->get_position_in_mother() + 
		dir*(0.5*maximum - start_frame->get_bounding_sphere_radius());
}
//------------------------------------------------------------------------------
Vec3 mean_of_positions_in_mother(const vector<Frame*> &frames) {

	if(frames.size() < 1) {
		stringstream info;
		info << __FILE__ << ", " << __LINE__ << "\n";
		info << "Expected at leas 1 frame to calculate mean position of frames, ";
		info << "but actual there are only " << frames.size() << " frames.";
		throw std::invalid_argument(info.str());		
	}

	Vec3 sum_pos = Vec3::null;

	for(Frame* frame : frames)
		sum_pos = sum_pos + frame->get_position_in_mother();

	return sum_pos/frames.size();
}
//------------------------------------------------------------------------------
bool positions_in_mother_are_too_close_together(const vector<Frame*> &frames) {

	if(frames.size() < 2)
		return false;
	else
		return spread_of_frame_position_in_mother(frames) < 
			Frame::minimal_structure_size;
}
//------------------------------------------------------------------------------
double spread_of_frame_position_in_mother(const vector<Frame*> &frames) {
	
	if(frames.size() < 2)
		return 0.0;

	const Vec3 mean_pos_in_mother = mean_of_positions_in_mother(frames);

	Vec3 u = Vec3::null;
	for(Frame* frame : frames) {
		const Vec3 r =  frame->get_position_in_mother() - mean_pos_in_mother;
		u = u + Vec3(
			r.x()*r.x(), 
			r.y()*r.y(), 
			r.z()*r.z()
		);
	}

	u = u/frames.size();
	return sqrt(u.norm());
}
//------------------------------------------------------------------------------
double bounding_sphere_radius(const vector<Frame*> &frames, const Vec3 center) {
	// Case: The new child is not enclosed by the previous sphere of this frame
	//-------             _______________                                     //
	//               ____/               \____                                //
	//            __/     \                   \__                             //
	//           /         \  radius             \                            //
    //          |           \  enclosing          |                           //
    //         |             \  all                |                          //
    //         |              \  (previous)        |          new child       //
    //        |  ___           \  children          |            ___          //
	//        |/     \          \                   |          /     \        //
    //        |       |          x------------------|---------|---x   |       //
    //        |\ ___ /         frame center         |          \ _._ /.       //
    //        | previous child   .                  |             .   . 
    //         | defining        .                 |              .   .
    //         |  the current    .                 |              .   .
	//          |  radius        .                |               .   .
	//           \__             .             __/                .   .
	//              \___         .        ____/                   .   .
	//                  \________._______/                        .   .
	//                           .                                .   .
	//                           \_________________  ____________/\_ _/
	//                                             \/               V
	//                                   new childs rel. pos    new childs
	//                                      to mother frame       radius
	//                                             \________  _______/
	//                                                      \/
	//                         new radius of sphere enclosing all children   
	// 
	// Here we have to update the radius of the sphere enclosing all children.
	//      
	// Case: The new child is enclosed by the previous sphere of this frame
	//-------
	//                    _______________
	//               ____/               \____                                //
	//            __/     \ radius            \__                             // 
	//           /         \ enclosing           \                            //
    //          |           \  all                |                        
    //         |             \  (previous)         |
    //         |              \  children           |            
    //        |                \           ___      |            
	//        |                 \        /     \    |          
    //        |                  x------|---x   |   |
    //        |            frame center  \ ___ /    |          
    //        |                  .      new child   |                 
    //         |                 .                 |                 
    //         |                 .                 |                  
	//          |                .                |                   
	//           \__             .             __/                    
	//              \___         .        ____/                       
	//                  \________________/                            
	//                                                              
	//                             
	// In this case the old radius remains because it is already enclosing the 
	// new child.

	double radius = 0.0;
	for(const Frame* child: frames) {
		const double radius_needed_for_child = 
			(center - child->get_position_in_mother()).norm() + 
			child->get_bounding_sphere_radius();

		if(radius_needed_for_child > radius)
			radius = radius_needed_for_child;
	}

	return radius;
}
//------------------------------------------------------------------------------
}// Frames