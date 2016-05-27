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
#define LIGHT "light.txt"
#define CAMERA "camera.txt"
#define OUTPUT "output.ppm"
#define CUSTOM "nudegirl4.stl"
#define TEXTURE_MAP "texturemap.txt"

//"brick","brown_wood","sand","grey_wood","chess_field"

int main(int argc, const char * argv[]) {
    std::ios_base::sync_with_stdio(false);
    SceneProcessor sceneProcessor(BACKGROUND_INTENSITY, 1);
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    sceneProcessor
                .scanDataFromMy(INPUT)
                .loadTextureFromPPMWithKey("images-2.txt", "brick")
                .loadTextureFromPPMWithKey("images-3.txt", "brown_wood")
                .loadTextureFromPPMWithKey("images-4.txt", "sand")
                .loadTextureFromPPMWithKey("images-5.txt", "grey_wood")
                .loadTextureFromPPMWithKey("images-6.txt", "chess_field")
                .addTextureMap(TEXTURE_MAP)
//                .scanDataFromASCISTL(CUSTOM)
                .scanLightData(LIGHT)
                .scanCameraData(CAMERA)
                .run()
                .printDataWithFormatPPM(OUTPUT)
    ;
    
    end = std::chrono::system_clock::now();
    int64_t elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    FILE* outtime = freopen("time.txt", "w", stdout);
    cout << "elapsed seconds: " << elapsed_milliseconds/1000 << endl;
    fclose(outtime);
    return 0;
}
