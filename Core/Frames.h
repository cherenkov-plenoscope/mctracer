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
    bool positions_in_mother_are_too_close_together(const vector<Frame*> &frames);
    Vec3 optimal_bounding_sphere_center(const vector<Frame*> &frames);
    Vec3 get_mean_pos_in_mother(const vector<Frame*> &frames);

    class TooFewFrames: public TracerException{
        using TracerException::TracerException;
    };
}

#endif // __FRAMES_H_INCLUDED__