//
//  lighting.cpp
//  raytracing
//
//  Created by Сергей Миллер on 15.04.16.
//  Copyright © 2016 sergmiller. All rights reserved.
//

#include "lighting.hpp"
#include <cassert>

LightSource::LightSource(ld _intensity, Point _centr): intensity(_intensity),  centr(_centr) {}

ld LightSource::calcIntensityInPoint(Point targetPoint, std::vector <Figure*>& figures) {
    Point ray = (targetPoint - centr) * EPS;
    if(ray == Point(0,0,0)) {
        return 0;
    }
    
    Point intersect(0,0,0);
    ld firstInterDist = 1e50;
    int firstInterNumb = -1;
    for(int i = 0;i < figures.size(); ++i) {
        std::pair<Status, Point> res = figures[i]->checkIntersect(ray, centr);
        Point p = res.second;
        
        Point beam = p - centr;
        
        if(beam.dist2() < firstInterDist) {
            firstInterDist = beam.dist2();
            firstInterNumb = i;
            intersect = p;
        }
    }
    
    Figure* firstInterFig = figures[firstInterNumb];

    

    Point diff = intersect - targetPoint;
    Point norm;
    Triangle* triangle = nullptr;
    if(dynamic_cast<Triangle*>(firstInterFig)) {
        triangle = static_cast<Triangle*>(firstInterFig);
        norm = triangle->getFrontSideNormalInPoint(Point(0,0,0));
    }
    if(diff.dist2() > EPS  || (triangle && scal(ray,norm) > 0)) {
        return 0;
    }

    return getIntensity(targetPoint, firstInterFig);
}

ld LightSource::getIntensity(Point targetPoint, Figure* figure) {
    Point ray = centr - targetPoint;
    ld dist2 = ray.dist2();
    Point norm = figure->getFrontSideNormalInPoint(targetPoint);
    ld cosNorm = scal(ray,norm)/sqrt(ray.dist2() * norm.dist2());
//    std::cout << (intensity * cosNorm)/dist2 << std::endl;
    return std::max((ld)0,(intensity * cosNorm)/dist2);
}