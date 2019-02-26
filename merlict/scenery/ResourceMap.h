// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_RESOURCEMAP_H_
#define SCENERY_RESOURCEMAP_H_

namespace merlict {

struct ResourceMap {
    struct NoSuchKey: public std::out_of_range {
        using out_of_range::out_of_range;
    };

    struct KeyAlreadyInUse: public std::invalid_argument {
        using invalid_argument::invalid_argument;
    };
};

}  // namespace merlict

#endif  // SCENERY_RESOURCEMAP_H_
