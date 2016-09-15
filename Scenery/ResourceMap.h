//=================================
// include guard
#ifndef __RESOURCEMAP_H_INCLUDED__
#define __RESOURCEMAP_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/TracerException.h"

class ResourceMap {
public:

    class NoSuchKey: public TracerException{
        using TracerException::TracerException;
    };

    class KeyAlreadyInUse: public TracerException{
        using TracerException::TracerException;
    };
};
#endif // __RESOURCEMAP_H_INCLUDED__