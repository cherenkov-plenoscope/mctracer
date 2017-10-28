//=================================
// include guard
#ifndef __RESOURCEMAP_H_INCLUDED__
#define __RESOURCEMAP_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies

class ResourceMap {
public:

    class NoSuchKey: public std::out_of_range {
        using out_of_range::out_of_range;
    };

    class KeyAlreadyInUse: public std::invalid_argument {
        using invalid_argument::invalid_argument;
    };
};
#endif // __RESOURCEMAP_H_INCLUDED__