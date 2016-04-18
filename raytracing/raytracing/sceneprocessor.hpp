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

#define MAX_COLOR 255
#define BACKGROUND_INTENSITY 0.1

class SceneProcessor {
private:
    Point observerPoint, controlPoint;
    pair <Point,Point> dim;
    pair <size_t,size_t> pixelSize;
    vector <std::shared_ptr<Figure> > figures;
    vector <vector<Color> > picture;
    vector <LightSource*> lights;
    LightSource backgroundLight;

    void scanData();
    Color calcPixelColor(int x, int y);
    void convertDataToFormatPPM();
public:
    SceneProcessor();
    void calculatePicture();
};

#endif /* sceneprocessor_hpp */

//1
//1
//0 255 0
//0 0 0 10000
//1
//4000 -8000 -2000 1e6
//-1000 0 750
//0 0 -1 1500
//0.8944 0.447 0 2000
//-50000 70000 9000