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
#include <map>
#include <cassert>
#include <string>
#include "geometrystructs.hpp"
#include "thread_pool.hpp"
#include "lighting.hpp"
#include "kdtree.hpp"

using std::string;
using std::vector;

#define BACKGROUND_INTENSITY 0.2
#define DEFALT_THREAD_NUMB 4
#define MAX_COLOR 255
#define YDIM 2000
#define XDIM 1500

class SceneProcessor {
private:
    std::map <string,int> texturesId;
    vector <Picture> textures;
    bool initCameraData;
    bool usingMultithreading;
    Point observerPoint, controlPoint;
    pair <Point,Point> dim;
    pair <size_t,size_t> pixelSize;
    vector <std::shared_ptr<Figure> > figures;
    std::shared_ptr<Kdtree> kdtree;
    Picture picture;
    vector <LightSource> lights;
    ld backgroundIntensity;
    
    Point leftBoundScene;
    Point rightBoundScene;
    
    Color calcColorInPoint(Point ray, Point start, int contribution, int depth);
    void calcPixel(int _x, int _y);
    static void __calcPixel(int _x, int _y, SceneProcessor* proc);
    ThreadPool pool;

    
    void getKeyPoints();
    void autoCameraPosition();
    void calcObserverPoint();
    void initKDtree();
public:
    SceneProcessor& loadTextureFromPPMWithKey(string name, string key);
    SceneProcessor& addTextureMap(string map);
    SceneProcessor& scanLightData(string lights);
    SceneProcessor& scanCameraData(string camera);
    SceneProcessor& scanDataFromMy(string input);
    SceneProcessor& scanDataFromASCISTL(string input);
    SceneProcessor& printDataWithFormatPPM(string output);
    SceneProcessor& run();
    
    SceneProcessor(ld intensity = 0, int threadNumb = DEFALT_THREAD_NUMB);
    ~SceneProcessor();
};

#endif /* sceneprocessor_hpp */
//-400 400 200 2e5