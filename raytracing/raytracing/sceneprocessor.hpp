//
//  sceneprocessor.hpp
//  raytracing
//
//  Created by Сергей Миллер on 17.04.16.
//  Copyright © 2016 sergmiller. All rights reserved.
//

#ifndef sceneprocessor_hpp
#define sceneprocessor_hpp

#include <stdio.h>
#include <vector>
#include <cassert>
#include "geometrystructs.hpp"
#include "lighting.hpp"

using namespace std;

typedef unsigned long long ui64;

#define MAX_COLOR 255
#define BACKGROUND_INTENSITY 0.1

class SceneProcessor {
private:
    Point observerPoint, controlPoint;
    pair <Point,Point> dim;
    pair <size_t,size_t> pixelSize;
    vector <Figure*> figures;
    vector <vector<Color> > picture;
    vector <LightSource*> lights;
    LightSource backgroundLight;

    void scanData();
    Color calcPixelColor(int i, int j);
    void convertDataToFormatPPM();
public:
    SceneProcessor();
    void calculatePicture();
};

#endif /* sceneprocessor_hpp */
