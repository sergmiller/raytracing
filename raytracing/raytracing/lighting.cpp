//
//  lighting.cpp
//  raytracing
//
//  Created by Сергей Миллер on 15.04.16.
//  Copyright © 2016 sergmiller. All rights reserved.
//

#include "lighting.hpp"
#include <cassert>

LightSource::LightSource(ld _intensity, Point _centr, Kdtree* _kdtree): intensity(_intensity),  centr(_centr), kdtree(_kdtree) {}

ld LightSource::findLitPoint(IntersectionData targetPointData) {
    Point targetPoint = point(targetPointData);
    Point ray = (targetPoint - centr);
    
    if(ray == Point(0,0,0)) {
        return 0;
    }
    
    IntersectionData realIntersectionData = kdtree->find(ray,centr);

    Point firstIntersection = point(realIntersectionData);

    Point diff = firstIntersection - targetPoint;

    if(diff.dist2() > EPS * EPS || status(realIntersectionData) != status(targetPointData) || figure(targetPointData) != figure(realIntersectionData)) {
        return 0;
    }

    return calcBrightness(targetPoint, figure(targetPointData));
}

ld LightSource::calcBrightness(Point targetPoint, std::shared_ptr<Figure> figure) {
    Point ray = centr - targetPoint;
    ld dist2 = ray.dist2();
    Point norm = figure->getFrontSideNormalInPoint(targetPoint);
    ld cosNorm = std::fabs(scal(ray,norm))/sqrt(ray.dist2() * norm.dist2());
    return (intensity * cosNorm)/dist2;
}