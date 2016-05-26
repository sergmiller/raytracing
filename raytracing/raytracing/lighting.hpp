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
#include "kdtree.hpp"


class LightSource {
public:
    LightSource(ld _intensity, Point _centr);
    ld findLitPoint(IntersectionData targetPointData, std::shared_ptr<Kdtree> kdtree);
    ld calcBrightness(Point targetPoint, std::shared_ptr<Figure> figure);
    ld intensity;
    Point centr;
};

#endif /* lighting_hpp */
