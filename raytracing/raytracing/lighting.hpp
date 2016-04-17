//
//  lighting.hpp
//  raytracing
//
//  Created by Сергей Миллер on 15.04.16.
//  Copyright © 2016 sergmiller. All rights reserved.
//

#ifndef lighting_hpp
#define lighting_hpp

#include <stdio.h>
#include <vector>
#include "geometrystructs.hpp"


class LightSource {
public:
    LightSource(ld _intensity, Point _centr);
    ld findLitPoint(std::tuple<Status,Point,Figure*> targetPointData, std::vector <Figure*>& figures);
    ld calcBrightness(Point targetPoint, Figure* figure);
    ld intensity;
    Point centr;
};

#endif /* lighting_hpp */
