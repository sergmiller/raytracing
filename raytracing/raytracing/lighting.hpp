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
#include "raytracingstructs.hpp"

class LightSource {
public:
    LightSource(){};
    LightSource(ld _intensity, Point centr);
    ld intensity;
    Point centr;
};

#endif /* lighting_hpp */
