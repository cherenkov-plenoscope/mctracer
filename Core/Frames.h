//=================================
// include guard
#ifndef __FRAMES_H_INCLUDED__
#define __FRAMES_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Frame.h"

namespace Frames {
	double spread_of_frame_position_in_mother(const vector<Frame*> &frames);
	Vec3 bounding_sphere_center(const vector<Frame*> &frames);
    Vec3 dumb_bounding_sphere_center(const vector<Frame*> &frames);
    Vec3 mean_of_positions_in_mother(const vector<Frame*> &frames);
    bool positions_in_mother_are_too_close_together(const vector<Frame*> &frames);
    double bounding_sphere_radius(const vector<Frame*> &frames, const Vec3 center);


    class TooFewFrames: public TracerException{
        using TracerException::TracerException;
    };
}

#endif // __FRAMES_H_INCLUDED__