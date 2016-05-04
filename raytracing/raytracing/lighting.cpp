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

ld LightSource::findLitPoint(std::tuple<Status,Point,std::shared_ptr<Figure>> targetPointData, std::vector <std::shared_ptr<Figure>>& figures) {
    std::shared_ptr<Figure> targetFigure = std::get<2>(targetPointData);
    Point targetPoint = std::get<1>(targetPointData);
    Status targetStatus = std::get<0>(targetPointData);
    Point ray = (targetPoint - centr);
    
    if(ray == Point(0,0,0)) {
        return 0;
    }
    
    std::tuple<Status,Point,std::shared_ptr<Figure>> realIntersectionData = kdtree->find(ray,centr);

    std::shared_ptr<Figure> firstInterFigure = std::get<2>(realIntersectionData);
    Point firstIntersection = std::get<1>(realIntersectionData);
    Status firstInterStatus = std::get<0>(realIntersectionData);
    
//    cout << endl;
//    firstIntersection.printPoint();
//    targetPoint.printPoint();
//    cout << endl;

    Point diff = firstIntersection - targetPoint;
//    cout << diff.dist2() << " " << ray.dist2() * EPS <<  endl;

    if(diff.dist2() > EPS * EPS || firstInterStatus != targetStatus || targetFigure != firstInterFigure) {
//        cout << "OK" << endl;
        return 0;
    }

    return calcBrightness(targetPoint, targetFigure);
}

ld LightSource::calcBrightness(Point targetPoint, std::shared_ptr<Figure> figure) {
    Point ray = centr - targetPoint;
    ld dist2 = ray.dist2();
    Point norm = figure->getFrontSideNormalInPoint(targetPoint);
    ld cosNorm = std::fabs(scal(ray,norm))/sqrt(ray.dist2() * norm.dist2());
//    std::cout << (intensity * cosNorm)/dist2 << std::endl;
    return (intensity * cosNorm)/dist2;
}