//
//  sceneprocessor.cpp
//  raytracing
//
//  Created by Сергей Миллер on 17.04.16.
//  Copyright © 2016 sergmiller. All rights reserved.
//

#include "sceneprocessor.hpp"

SceneProcessor::SceneProcessor():backgroundLight(LightSource(BACKGROUND_INTENSITY, Point(0,0,0))) {};

void SceneProcessor::scanData() {
    int n;
    cin >> n;
    for (int i = 0;i < n;++i) {
        int type;
        int color[3];
        cin >> type;
        for(int i = 0;i < 3;++i) {
            cin >> color[i];
        }
        if(!type) {
            Point v[3];
            for(int i = 0;i < 3;++i) {
                cin >> v[i].x >> v[i].y >> v[i].z;
            }
            figures.push_back(new Triangle(color,v));
        } else {
            Point c;
            ld r;
            cin >> c.x >> c.y >> c.z >> r;
            figures.push_back(new Sphere(color,c,r));
        }
    }
    
    int k;
    cin >> k;
    for(int i = 0;i < k;++i) {
        Point c;
        ld intense;
        cin >> c.x >> c.y >> c.z >> intense;
        lights.push_back(new LightSource(intense,c));
    }
    
    cin >> controlPoint.x >> controlPoint.y >> controlPoint.z;
    cin >> dim.first.x >> dim.first.y >> dim.first.z >> pixelSize.first;
    cin >> dim.second.x >> dim.second.y >> dim.second.z >> pixelSize.second;
    cin >> observerPoint.x >> observerPoint.y >> observerPoint.z;
}


void SceneProcessor::convertDataToFormatPPM() {
    cout << "P3\n";
    cout << pixelSize.second << " " <<  pixelSize.first << "\n";
    cout << MAX_COLOR << "\n";
    for(int i = 0;i < pixelSize.first;++i) {
        for(int j = 0; j < pixelSize.second;++j) {
            cout << picture[i][j].R << " " << picture[i][j].G << " " << picture[i][j].B;
            cout << "\n";
        }
    }
}


Color SceneProcessor::calcPixelColor(int x, int y) {
    Color color;
    Point pixel = controlPoint + ((dim.first * (ld)x) + (dim.second * (ld)y));
    Point ray = pixel - observerPoint;
    
    if(ray == Point(0,0,0)) {
        return color;
    }
    
    std::tuple<Status,Point,Figure*> intersectionData = observerPoint.findFirstIntersect(figures, ray, 1);

    ld brightness = 0;
    
    if(std::get<0>(intersectionData) != NOT_INTERSECT) {
        color = std::get<2>(intersectionData)->getColor();
        for(int i = 0;i < lights.size(); ++i) {
            brightness += lights[i]->findLitPoint(intersectionData, figures);
        }
//        cout << brightness << endl;
    }

    return color * (brightness + backgroundLight.intensity);
}

void SceneProcessor::calculatePicture() {
    scanData();
    
    picture.resize(pixelSize.first, vector <Color> (pixelSize.second));
    
    for(int i =0;i < pixelSize.first;++i) {
        for(int j = 0;j < pixelSize.second;++j) {
            picture[i][j] = calcPixelColor(i,j);
        }
    }
    
    convertDataToFormatPPM();
//    (static_cast<Triangle*>(figures[1]))->normalToFrontSide.printPoint();
}

