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
#include <vector>
#include <cmath>
#include <iostream>

typedef long double ld;

#define EPS (ld)1e-9

enum status {
    NOT_INTERSECT,
    FRONT_SIDE_INTERSECT,
    BACK_SIDE_INTERSECT
};

struct Point {
    ld x,y,z;
    Point(){
        x = y = z = 0;
    }
    Point(ld _x, ld _y, ld _z) {
        x = _x;
        y = _y;
        z = _z;
    }
    const Point operator +(const Point& p) const{
        return Point(x + p.x, y + p.y, z + p.z);
    }
    
    const Point operator -(const Point& p) const{
        return Point(x - p.x, y - p.y, z - p.z);
    }
    
    const Point operator *(const ld& m) const{
        return Point(m * x, m * y, m * z);
    }
    
    ld dist2() {
        return x*x + y*y + z*z;
    }
    
    const bool operator ==(const Point& p) const {
        Point d = *this - p;
        return d.dist2() < (EPS * EPS);
    }
    void printPoint() {
        std::cout << "***********" << std::endl;
        std::cout << x << " " << y << " " << z << std::endl;
        std::cout << "***********" << std::endl;
    }
};

struct Color {
    int R,G,B;
    Color() {
        R = G = B = 0;
    }
    Color(int c[3]) {
        R = c[0];
        G = c[1];
        B = c[2];
    }
    void operator =(const Color& color) {
        R = color.R;
        G = color.G;
        B = color.B;
    }
};


//Point pInf(1e100,1e100,1e100);

class Figure{
public:
    virtual std::pair <status,Point> checkIntersect(Point ray, Point start) = 0;
    Color color;
};

class Triangle: public Figure{
public:
    Triangle(){};
    Triangle(int color[3], Point _v[3], Point norm = Point(0,0,0));
    std::pair <status,Point> checkIntersect(Point ray, Point start);
    Point v[3];
    Point norm;
};

class Sphere: public Figure{
public:
    Sphere(){};
    Sphere(int color[3],Point _centr, ld _radius);
    std::pair <status,Point> checkIntersect(Point ray, Point start);
    ld radius;
    Point centr;
};

#endif /* raytracingstructs_hpp */
