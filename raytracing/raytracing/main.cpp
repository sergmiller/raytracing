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
#include <chrono>

#include "sceneprocessor.hpp"

#define INPUT "input.txt"
#define GNOME "gnomeASCI.stl"
#define CAMERA "cameraData.txt"
#define OUTPUT "output.ppm"

int main(int argc, const char * argv[]) {
    FILE* out = freopen(OUTPUT, "w", stdout);
    std::ios_base::sync_with_stdio(false);
    SceneProcessor sceneProcessor(INPUT,CAMERA,OUTPUT,BACKGROUND_INTENSITY);
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    sceneProcessor.calculatePicture();
    end = std::chrono::system_clock::now();
    int64_t elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    fclose(out);
    freopen("out.txt", "w", stdout);
    std::cout << "elapsed seconds: " << elapsed_milliseconds/1000 << std::endl;
    return 0;
}
