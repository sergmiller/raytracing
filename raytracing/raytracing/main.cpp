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

#define BACKGROUND_INTENSITY 0.2
#define INPUT "input.txt"
#define GNOME "gnomeASCI.stl"
#define LIGHT "light.txt"
#define CAMERA "camera.txt"
#define OUTPUT "output.ppm"
#define CUSTOM "chri.stl"

int main(int argc, const char * argv[]) {
    std::ios_base::sync_with_stdio(false);
    SceneProcessor sceneProcessor(BACKGROUND_INTENSITY);
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    sceneProcessor
//                    .scanDataFromMy(INPUT)
                    .scanDataFromASCISTL(CUSTOM)
                    .scanLightData(LIGHT)
//                    .scanCameraData(CAMERA)
                    .run()
                    .printDataWithFormatPPM(OUTPUT);
    
    end = std::chrono::system_clock::now();
    int64_t elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    FILE* outtime = freopen("time.txt", "w", stdout);
    cout << "elapsed seconds: " << elapsed_milliseconds/1000 << endl;
    fclose(outtime);
    return 0;
}
