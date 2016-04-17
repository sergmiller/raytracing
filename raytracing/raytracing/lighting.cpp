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

ld LightSource::findLitPoint(std::tuple<Status,Point,Figure*> targetPointData, std::vector <Figure*>& figures) {
    Figure* targetFigure = std::get<2>(targetPointData);
    Point targetPoint = std::get<1>(targetPointData);
    Status targetStatus = std::get<0>(targetPointData);
    Point ray = (targetPoint - centr);
    if(ray == Point(0,0,0)) {
        return 0;
    }
    
    std::tuple<Status,Point,Figure*> realIntersectionData = centr.findFirstIntersect(figures, ray, 0);

    Figure* firstInterFigure = std::get<2>(realIntersectionData);
    Point firstIntersection = std::get<1>(realIntersectionData);
    Status firstInterStatus = std::get<0>(realIntersectionData);
    
//    cout << endl;
//    firstIntersection.printPoint();
//    targetPoint.printPoint();
//    cout << endl;

    Point diff = firstIntersection - targetPoint;
//    cout << diff.dist2() << " " << ray.dist2() * EPS <<  endl;

    if(diff.dist2() > EPS * EPS * ray.dist2() || firstInterStatus != targetStatus) {
//        cout << "OK" << endl;
        return 0;
    }

    return calcBrightness(targetPoint, firstInterFigure);
}

ld LightSource::calcBrightness(Point targetPoint, Figure* figure) {
    Point ray = centr - targetPoint;
    ld dist2 = ray.dist2();
    Point norm = figure->getFrontSideNormalInPoint(targetPoint);
    ld cosNorm = std::abs(scal(ray,norm))/sqrt(ray.dist2() * norm.dist2());
//    std::cout << (intensity * cosNorm)/dist2 << std::endl;
    return (intensity * cosNorm)/dist2;
}