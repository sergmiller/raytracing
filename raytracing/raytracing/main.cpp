//
//  main.cpp
//  raytracing
//
//  Created by Сергей Миллер on 10.04.16.
//  Copyright © 2016 sergmiller. All rights reserved.
//

#include <iostream>
#include <vector>
#include <cassert>

#include "sceneprocessor.hpp"

#define INPUT "input.txt"
#define OUTPUT "output.ppm"

int main(int argc, const char * argv[]) {
    std::ios_base::sync_with_stdio(false);
    freopen(INPUT, "r", stdin);
    freopen(OUTPUT, "w", stdout);
    SceneProcessor sceneProcessor;
    sceneProcessor.calculatePicture();
    return 0;
}
