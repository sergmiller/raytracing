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
#include "kdtree.hpp"

using namespace std;

#define MAX_COLOR 255
#define BACKGROUND_INTENSITY 0.1

class SceneProcessor {
private:
    Point observerPoint, controlPoint;
    pair <Point,Point> dim;
    pair <size_t,size_t> pixelSize;
    vector <std::shared_ptr<Figure> > figures;
    Kdtree* kdtree;
    vector <vector<Color> > picture;
    vector <LightSource*> lights;
    ld backgroundIntensity;

    void scanData();
    Color calcPixelColor(int x, int y);
    void convertDataToFormatPPM();
public:
    SceneProcessor(ld intensity = 0);
    void calculatePicture();
};

#endif /* sceneprocessor_hpp */
//-400 400 200 2e5