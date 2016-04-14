//
//  raytracingstructs.cpp
//  raytracing
//
//  Created by Сергей Миллер on 10.04.16.
//  Copyright © 2016 sergmiller. All rights reserved.
//

#include "raytracingstructs.hpp"

ld scal(point& p1, point& p2) {
    return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}

ld det(ld a, ld b, ld c, ld d) {
    return a * d - b * c;
}

point vect(point& a, point& b) {
    point p;
    p.x = det(a.y, a.z, b.y, b.z);
    p.y = det(a.z, a.x, b.z, b.x);
    p.z = det(a.x, a.y, b.x, b.y);
    return p;
}

triangle::triangle(int _color[3], point _v[3], point norm): norm(norm) {
    for(int i = 0;i < 3;++i) {
        color[i] = _color[i];
        v[i] = _v[i];
    }
}

//sphere::sphere(int _color[3],point _c, ld _r): c(_c), r(_r) {
//    for(int i = 0;i < 3;++i) {
//        color[i] = _color[i];
//    }
//}

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

std::pair <status,point> triangle::checkIntersect(point ray, point start) {
    point intersect;
    if(ray.x == ray.y && ray.x == 0) {
//        printPoint(ray);
//        printPoint(start);
           // return std::make_pair(NOT_INTERSECT,intersect);
    }
    point v_01 = v[1] - v[0];
    point v_02 = v[2] - v[0];

    //case if triangle with measure 0
    if(vect(v_01, v_02) == point(0,0,0)) {
//        if(ray.x == ray.y && ray.x == 0) {
//            std::cout << "  //case if triangle with measure 0" << std::endl;
//        }
        return std::make_pair(NOT_INTERSECT,intersect);
    }
    
    point norm = vect(v_01, v_02);
    
    ld offset = -scal(norm, v[0]);
    
    //case if ray in triangle's flat
    if(std::abs(scal(norm,ray)) < EPS) {
//        if(ray.x == ray.y && ray.x == 0) {
//            std::cout << "//case if ray in triangle's flat" << std::endl;
//        }
        return std::make_pair(NOT_INTERSECT,intersect);
    }
    
    ld t = -(scal(norm, start) + offset)/(scal(norm,ray));
    
    //case if triangle in other side of ray
    if(t <= 1) {
//        if(ray.x == ray.y && ray.x == 0) {
//            std::cout << " //case if triangle in other side of ray" << std::endl;
//        }
        return std::make_pair(NOT_INTERSECT,intersect);
    }
    
    intersect = start + ray * t;
    
    //check that intersect inside triangle:
    
    point v_0inter = intersect - v[0];
    
    ld m[2][2];
    m[0][0] = v_01.x;
    m[0][1] = v_02.x;
    m[1][0] = v_01.y;
    m[1][1] = v_02.y;
    
    ld vc[2];
    vc[0] = v_0inter.x;
    vc[1] = v_0inter.y;
    std::pair <ld,ld> coord = solveMatrix(m, vc);
    
    ld line1[3] = {1,0,0};
    ld line2[3] = {0,1,0};
    ld line3[3] = {1,1,-1};
    
    if(sgn(coord,line1) > 0 && sgn(coord, line2) > 0 && sgn(coord, line3) < 0) {
        return std::make_pair(FRONT_SIDE_INTERSECT, intersect);
    } else {
        //case if intersection outside of triangle
//        if(ray.x == ray.y && ray.x == 0) {
//            std::cout << "//case if intersection outside of triangle" << std::endl;
//        }
        return std::make_pair(NOT_INTERSECT, intersect);
    }
}














