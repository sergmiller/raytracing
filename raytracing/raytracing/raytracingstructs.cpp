//
//  raytracingstructs.cpp
//  raytracing
//
//  Created by Сергей Миллер on 10.04.16.
//  Copyright © 2016 sergmiller. All rights reserved.
//

#include "raytracingstructs.hpp"


triangle::triangle(int _color[3], point _v[3], point norm): norm(norm) {
    for(int i = 0;i < 3;++i) {
        color[i] = _color[i];
        v[i] = _v[i];
    }
}

sphere::sphere(int _color[3],point _c, ld _r): c(_c), r(_r) {
    for(int i = 0;i < 3;++i) {
        color[i] = _color[i];
    }
}