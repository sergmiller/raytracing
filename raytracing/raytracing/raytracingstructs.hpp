//
//  raytracingstructs.hpp
//  raytracing
//
//  Created by Сергей Миллер on 10.04.16.
//  Copyright © 2016 sergmiller. All rights reserved.
//

#ifndef raytracingstructs_hpp
#define raytracingstructs_hpp

#include <stdio.h>

typedef long double ld;

#define EPS (ld)1e-18

struct point {
    ld x,y,z;
    point(){
        x = y = z = 0;
    }
    point(ld _x, ld _y, ld _z) {
        x = _x;
        y = _y;
        z = _z;
    }
    const point operator +(const point& p) const{
        return point(x + p.x, y + p.y, z + p.z);
    }
    
    const point operator -(const point& p) const{
        return point(x - p.x, y - p.y, z - p.z);
    }
    
    const point operator *(const ld& m) const{
        return point(m * x, m * y, m * z);
    }
    
    ld dist2() {
        return x*x + y*y + z*z;
    }
    
    const bool operator ==(const point& p) const {
        point d = *this - p;
        return d.dist2() < EPS;
    }
};

point pInf(1e100,1e100,1e100);

class obj{
public:
    point checkIntersect(point ray, point start);
    int color[3];
};

class triangle: public obj{
public:
    triangle();
    triangle(int color[3], point _v[3], point norm = point(0,0,0));
    point v[3];
    point norm;
};

class sphere: public obj{
public:
    sphere();
    sphere(int color[3],point _c, ld _r);
    ld r;
    point c;
};

#endif /* raytracingstructs_hpp */
