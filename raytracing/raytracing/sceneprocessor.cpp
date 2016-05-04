//
//  sceneprocessor.cpp
//  raytracing
//
//  Created by Сергей Миллер on 17.04.16.
//  Copyright © 2016 sergmiller. All rights reserved.
//

#include "sceneprocessor.hpp"

SceneProcessor::SceneProcessor(ld intensity):backgroundIntensity(intensity) {};

void SceneProcessor::scanData() {
    int n;
    cin >> n;
    for (int i = 0;i < n;++i) {
        int type;
        int color[3];
        cin >> type;
        for(int i = 0;i < 3;++i) {
            cin >> color[i];
            color[i]  = std::max(1,color[i]);
        }
        if(!type) {
            Point v[3];
            for(int i = 0;i < 3;++i) {
                cin >> v[i].x >> v[i].y >> v[i].z;
            }
            figures.push_back(std::shared_ptr<Figure>(new Triangle(color,v)));
        } else {
            Point c;
            ld r;
            cin >> c.x >> c.y >> c.z >> r;
            figures.push_back(std::shared_ptr<Figure>(new Sphere(color,c,r)));
        }
    }
    
    /***init kdtree******/
    
    ld l[3] = {1e100,1e100,1e100};
    ld r[3] = {-1e100,-1e100,-1e100};
    
    for(int i = 0; i <figures.size(); ++i) {
        for(int d = 0; d < 3;++d) {
            l[d] = fmin(l[d],figures[i]->getLeftBound(d));
            r[d] = fmax(r[d],figures[i]->getRightBound(d));
        }
    }
    
    Point leftBound(l), rightBound(r);
    
    kdtree = new Kdtree(figures,leftBound,rightBound);
    /********************/
    
    int k;
    cin >> k;
    for(int i = 0;i < k;++i) {
        Point c;
        ld intense;
        cin >> c.x >> c.y >> c.z >> intense;
        lights.push_back(new LightSource(intense,c, kdtree));
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
    
    std::tuple<Status,Point,std::shared_ptr<Figure>> intersectionData = kdtree->find(ray, pixel);

    ld brightness = 0;
    
    if(std::get<0>(intersectionData) != NOT_INTERSECT) {
        color = std::get<2>(intersectionData)->getColor();
        for(int i = 0;i < lights.size(); ++i) {
            brightness += lights[i]->findLitPoint(intersectionData, figures);
        }
//        cout << brightness << endl;
    }

    return color * (brightness + backgroundIntensity);
}

void SceneProcessor::calculatePicture() {
    scanData();
    
    picture.resize(pixelSize.first, vector <Color> (pixelSize.second));

    
    for(int i =0;i < pixelSize.first;++i) {
        for(int j = 0;j < pixelSize.second;++j) {
//            cout << i << " " << j << endl;
            picture[i][j] = calcPixelColor(i,j);
        }
    }
    
    convertDataToFormatPPM();
//    (static_cast<Triangle*>(figures[1]))->normalToFrontSide.printPoint();
}

