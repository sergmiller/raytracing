//
//  geometrystructs.hpp
//  raytracing
//
//  Created by Сергей Миллер on 10.04.16.
//  Copyright © 2016 sergmiller. All rights reserved.
//

#ifndef geometrystructs_hpp
#define geometrystructs_hpp

#include <stdio.h>
#include <vector>
#include <cmath>
#include <iostream>

typedef long double ld;

using std::fmax;
using std::fmin;
using std::pair;
using std::cin;
using std::cout;
using std::endl;
using std::make_pair;

#define EPS (ld)1e-9

enum Status {
    NOT_INTERSECT,
    FRONT_SIDE_INTERSECT,
    BACK_SIDE_INTERSECT
};

struct Point;
struct Color;
class Figure;
class Point;
class Sphere;

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
    Point(ld d[3]) {
        x = d[0];
        y = d[1];
        z = d[2];
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
    
    const ld operator[](const int i) const {
        switch(i) {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            default:
                return 0;
        }
    }
    
    ld dist2() {
        return x*x + y*y + z*z;
    }
    
    const bool operator ==(const Point& p) const {
        Point d = *this - p;
        return d.dist2() < (EPS * EPS);
    }
    void printPoint() {
        cout << "***********" << endl;
        cout << x << " " << y << " " << z << endl;
        cout << "***********" << endl;
    }
    
//    std::tuple <Status,Point,std::shared_ptr<Figure>> findFirstIntersect(Kdtree* kdtree,Point ray,Point start);
};

struct Color {
    int R,G,B;
    Color() {
        R = G = B = 1;
    }
    Color(int _R, int _G, int _B): R(_R), G(_G), B(_B) {}
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
    const Color operator *(const ld& m) const{
        return Color(int(fmax(1,fmin(255.0,m * R))), int(fmax(1,fmin(255,m * G))), int(fmax(1,fmin(255,m * B))));
    }
};

class Figure{
public:
    virtual pair <Status,Point> checkIntersect(Point ray, Point start) = 0;
    virtual Point getFrontSideNormalInPoint(Point p) = 0;
    Color getColor() { return color; }
    ld getRightBound(int dim) { return rightBound[dim];}
    ld getLeftBound(int dim) { return rightBound[dim];}
    Point getRightBound() { return rightBound;}
    Point getLeftBound() { return rightBound;}
protected:
    Color color;
    Point rightBound;
    Point leftBound;
};

class Triangle: public Figure{
public:
    Triangle(int color[3], Point _v[3], Point norm = Point(0,0,0));
    pair <Status,Point> checkIntersect(Point ray, Point start);
    Point getFrontSideNormalInPoint(Point p);
    Point normalToFrontSide;
private:
    Point v[3];
};

class Sphere: public Figure{
public:
    Sphere(int color[3],Point _centr, ld _radius);
    pair <Status,Point> checkIntersect(Point ray, Point start);
    Point getFrontSideNormalInPoint(Point p);
private:
    Point centr;
    ld radius;
};

ld det(ld a, ld b, ld c, ld d);
ld det(ld a0[3], ld a1[3], ld a2[3]);
ld scal(Point& p1, Point& p2);
Point vect(Point& a, Point& b);

void packingRatio(pair<ld,ld>& r);

ld getSurface(Point l, Point r);

typedef std::tuple<Status,Point,std::shared_ptr<Figure>> IntersectionData;

#define status(inter) std::get<0>(inter)
#define point(inter) std::get<1>(inter)
#define figure(inter) std::get<2>(inter)

#endif /* geometrystructs_hpp */
