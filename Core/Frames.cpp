#include "Core/Frames.h"
namespace Frames {
//------------------------------------------------------------------------------
Vec3 optimal_bounding_sphere_center(const vector<Frame*> &frames) {

	if(frames.size() < 1) {
		stringstream info;
		info << __FILE__ << ", " << __LINE__ << "\n";
		info << "Expected at least 1 frame to calculate center position of frames, ";
		info << "but actual there are only " << frames.size() << " frames.";
		throw TooFewFrames(info.str());		
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
					frames[i]->contour_radius() + 
					frames[j]->contour_radius();

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
		dir*(0.5*maximum - start_frame->contour_radius());
}
//------------------------------------------------------------------------------
Vec3 get_mean_pos_in_mother(const vector<Frame*> &frames) {

	if(frames.size() < 1) {
		stringstream info;
		info << __FILE__ << ", " << __LINE__ << "\n";
		info << "Expected at leas 1 frame to calculate mean position of frames, ";
		info << "but actual there are only " << frames.size() << " frames.";
		throw TooFewFrames(info.str());		
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

	const Vec3 mean_pos_in_mother = get_mean_pos_in_mother(frames);

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
	const double spread = sqrt(u.norm());
	return spread < Frame::minimal_structure_size;
}
//------------------------------------------------------------------------------
}// Frames