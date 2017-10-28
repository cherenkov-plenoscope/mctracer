//=================================
// include guard
#ifndef __TOOLS_Tuple3_H_INCLUDED__
#define __TOOLS_Tuple3_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies  

struct Tuple3 {

    double x,y,z;

    Tuple3() 
    {
        x = 0.0;
        y = 0.0;
        z = 0.0;
    }
    Tuple3(double _x, double _y, double _z) 
    {
        x = _x;
        y = _y;
        z = _z;
    }
    bool operator == (const Tuple3& eq)const{
        return x == eq.x && y == eq.y && z == eq.z;
    }
    
    bool operator != (const Tuple3& eq)const {
        return x != eq.x || y != eq.y || z != eq.z;
    }
};

#endif // __TOOLS_Tuple3_H_INCLUDED__