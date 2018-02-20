// Copyright 2014 Sebastian A. Mueller
#ifndef TOOLS_TUPLE3_H_
#define TOOLS_TUPLE3_H_

struct Tuple3 {
    double x, y, z;

    Tuple3() {
        x = 0.0;
        y = 0.0;
        z = 0.0;
    }
    Tuple3(double _x, double _y, double _z) {
        x = _x;
        y = _y;
        z = _z;
    }

    bool operator == (const Tuple3& eq)const {
        return x == eq.x && y == eq.y && z == eq.z;
    }

    bool operator != (const Tuple3& eq)const {
        return x != eq.x || y != eq.y || z != eq.z;
    }
};

#endif  // TOOLS_TUPLE3_H_
