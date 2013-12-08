//=================================
// include guard
#ifndef __LIGHTSOURCE_H_INCLUDED__
#define __LIGHTSOURCE_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "CameraDevice.h"

//=================================
class LightSource: public CameraDevice{
private:
double OpeningAngle_in_rad;
/*
          camera direction
                /\
          \     |     /   In this cone photons will be 
           \    |____/    emitted.
            \   | a /
             \  |  /
              \ | /
              _\|/_
             /light\
             |sour-|
             \_ce__/

 a = OpeningAngle_in_rad
*/
public:
//======================================================================
void set_opening_angle(double new_OpeningAngle_in_rad);
//======================================================================
void radiate(CartesianFrame* world,GlobalSettings* settings)const;
//======================================================================
std::string get_bulb_string()const;
//======================================================================
void disp_bulb()const;
//======================================================================
};
#endif // __LIGHTSOURCE_H_INCLUDED__
