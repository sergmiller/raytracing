//
//  geometrystructs.cpp
//  raytracing
//
//  Created by Сергей Миллер on 10.04.16.
//  Copyright © 2016 sergmiller. All rights reserved.
//

#include "geometrystructs.hpp"

ld det(ld a, ld b, ld c, ld d) {
    return a * d - b * c;
}

ld scal(Point p1, Point p2) {
    return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}


Point vect(Point a, Point b) {
    Point p;
    p.x = det(a.y, a.z, b.y, b.z);
    p.y = det(a.z, a.x, b.z, b.x);
    p.z = det(a.x, a.y, b.x, b.y);
    return p;
}

Triangle::Triangle(int _color[3], Point _v[3], Point normal): normalToFrontSide(normal) {
    color = Color(_color);
    for(int i = 0;i < 3;++i) {
        v[i] = _v[i];
    }
    if(normalToFrontSide == Point(0,0,0)) {
        Point vec1 = v[1] - v[0];
        Point vec2 = v[2] - v[0];
        
        normalToFrontSide = vect(vec1, vec2);
    }
}

Sphere::Sphere(int _color[3],Point _centr, ld _radius): centr(_centr), radius(_radius) {
    color = Color(_color);
}

std::pair<ld,ld> solveMatrix(ld m[2][2], ld v[2]) {
    ld d = det(m[0][0], m[0][1], m[1][0],m[1][1]);
    ld d1 = det(v[0], m[0][1], v[1], m[1][1]);
    ld d2 = det(m[0][0], v[0], m[1][0], v[1]);
    return std::make_pair(d1/d, d2/d);
}

int sgn(std::pair<ld,ld> p, ld line[3]) {
    ld res = line[0] * p.first + line[1] * p.second  + line[2];
    if(std::abs(res) < EPS) {
        return 0;
    } else {
        return pow(-1, (res < 0));
    }
}

std::pair <Status,Point> Triangle::checkIntersect(Point ray, Point start, ld offsetMult) {
    Point intersect;
//    if(ray.x == ray.y && ray.x == 0) {
//        printPoint(ray);
//        printPoint(start);
//        return std::make_pair(NOT_INTERSECT,intersect);
//    }
    Point v_01 = v[1] - v[0];
    Point v_02 = v[2] - v[0];

    //case if triangle with measure 0
    if(vect(v_01, v_02) == Point(0,0,0)) {
        return make_pair(NOT_INTERSECT,intersect);
    }
    
    ld offset = -scal(normalToFrontSide, v[0]);
    
    //case if ray in triangle's flat
    if(abs(scal(normalToFrontSide,ray)) < EPS) {
        return make_pair(NOT_INTERSECT,intersect);
    }
    
    ld t = -(scal(normalToFrontSide, start) + offset)/(scal(normalToFrontSide,ray));
    
    //case if triangle in other side of ray
    if(t <= offsetMult) {
        return make_pair(NOT_INTERSECT,intersect);
    }
    
    intersect = start + ray * t;
    
    //check that intersection inside triangle:
    
    Point v_0inter = intersect - v[0];
    
    ld m[2][2];
    m[0][0] = v_01.x;
    m[0][1] = v_02.x;
    m[1][0] = v_01.y;
    m[1][1] = v_02.y;
    
    ld vc[2];
    vc[0] = v_0inter.x;
    vc[1] = v_0inter.y;
    pair <ld,ld> coord = solveMatrix(m, vc);
    
    ld line1[3] = {1,0,0};
    ld line2[3] = {0,1,0};
    ld line3[3] = {1,1,-1};
    
    if(sgn(coord,line1) > 0 && sgn(coord, line2) > 0 && sgn(coord, line3) < 0) {
        Status intersectionStatus = (scal(normalToFrontSide, ray) > 0 ? FRONT_SIDE_INTERSECT : BACK_SIDE_INTERSECT);
        return make_pair(intersectionStatus, intersect);
    } else {
        //case if intersection outside of triangle
        return make_pair(NOT_INTERSECT, intersect);
    }
}

std::pair <Status,Point> Sphere::checkIntersect(Point ray, Point start, ld offsetMult) {
    Point intersect;
    ld offset = -scal(ray, centr);
    ld t = -(scal(ray, start) + offset)/(ray.dist2());
    
    //case if Sphere in other side of ray
    if(t <= offsetMult) {
        return make_pair(NOT_INTERSECT,intersect);
    }
    
    intersect = start + ray * t;//WRONG!!!
    
    Point cv_inter = intersect - centr;
    
    if(cv_inter.dist2() < radius * radius) {
        Status intersectionStatus = (scal(intersect - centr, ray) > 0 ? FRONT_SIDE_INTERSECT : BACK_SIDE_INTERSECT);
        return make_pair(intersectionStatus, intersect);
    } else {
        return make_pair(NOT_INTERSECT, intersect);
    }
}

Point Triangle::getFrontSideNormalInPoint(Point p) {
    return normalToFrontSide;
}

Point Sphere::getFrontSideNormalInPoint(Point p) {
    return p - centr;
}

std::tuple <Status,Point,Figure*> Point::findFirstIntersect(std::vector <Figure*>& figures, Point ray, ld offsetMult) {
    Point intersection;
    Figure* intersectFigure = nullptr;
    ld intersectDist2 = INFINITY;
    Status intersectionStatus = NOT_INTERSECT;
    
    for(Figure* figure : figures) {
        pair <Status, Point> intersectData = figure->checkIntersect(ray, *this, offsetMult);
        if(intersectData.first == NOT_INTERSECT) {
            continue;
        }
        
        Point vecToCurrentIntersection = intersectData.second - *this;
        
        ld dist2ToCurrentIntersection = vecToCurrentIntersection.dist2();
        
        if(dist2ToCurrentIntersection < intersectDist2) {
            intersectionStatus = intersectData.first;
            intersectDist2 = dist2ToCurrentIntersection;
            intersection = intersectData.second;
            intersectFigure = figure;
        }
    }
    
    return std::make_tuple(intersectionStatus, intersection, intersectFigure);
}











