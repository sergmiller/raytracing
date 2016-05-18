//
//  sceneprocessor.cpp
//  raytracing
//
//  Created by Сергей Миллер on 17.04.16.
//  Copyright © 2016 sergmiller. All rights reserved.
//

#include "sceneprocessor.hpp"

SceneProcessor::SceneProcessor(std::string _fileName, std::string _camera, std::string _out, ld intensity):backgroundIntensity(intensity), inputFileName(_fileName), cameraData(_camera), outputFileName(_out) {};

void SceneProcessor::scanDataFromASCISTL() {
    FILE* input = freopen(inputFileName.data(), "r", stdin);
    string s;
    cin >> s;
    if(s != "solid") {
        throw new std::bad_function_call;
    }
    cin >> s >> s;
    int color[3] = {192,192,192};
    while(s != "endsolid") {
        cin >> s;
        Point norm;
        cin >> norm.x >> norm.y >> norm.z;
        cin >> s >> s;
        Point v[3];
        for(int i = 0;i < 3;++i) {
            cin >> s;
            cin >> v[i].x >> v[i].y >> v[i].z;
        }
        
        figures.push_back(std::shared_ptr<Figure>(new Triangle(Color(color),v)));
        cin >> s >> s >> s;
    }
    
    fclose(input);
    
}

void SceneProcessor::scanDataFromMy() {
    FILE* input = freopen(inputFileName.data(), "r", stdin);
    int n;
    cin >> n;
    for (int i = 0;i < n;++i) {
        int type;
        int color[3];
        cin >> type;
        for(int i = 0;i < 3;++i) {
            cin >> color[i];
            color[i]  = std::min(255,std::max(1,color[i]));
        }
        if(!type) {
            Point v[3];
            for(int i = 0;i < 3;++i) {
                cin >> v[i].x >> v[i].y >> v[i].z;
            }
            figures.push_back(std::shared_ptr<Figure>(new Triangle(Color(color),v)));
        } else {
            Point c;
            ld r;
            cin >> c.x >> c.y >> c.z >> r;
            figures.push_back(std::shared_ptr<Figure>(new Sphere(Color(color),c,r)));
        }
    }
    
    fclose(input);
}

void SceneProcessor::convertDataToFormatPPM() {
    FILE* out = freopen(outputFileName.data(), "w", stdout);
    cout << "P3\n";
    cout << pixelSize.second << " " <<  pixelSize.first << "\n";
    cout << MAX_COLOR << "\n";
    for(int i = 0;i < pixelSize.first;++i) {
        for(int j = 0; j < pixelSize.second;++j) {
            cout << picture[i][j].R << " " << picture[i][j].G << " " << picture[i][j].B;
            cout << "\n";
        }
    }
    fclose(out);
}


Color SceneProcessor::calcPixelColor(int _x, int _y) {
    Color color;
    Point pixel = controlPoint + ((dim.first * (ld)_x) + (dim.second * (ld)_y));
    Point ray = pixel - observerPoint;
    
    if(ray == Point(0,0,0)) {
        return color;
    }
    
    IntersectionData intersectionData = kdtree->find(ray, pixel);

    ld brightness = 0;
    
    if(status(intersectionData) != NOT_INTERSECT) {
        color = figure(intersectionData)->getColor();
        for(int i = 0;i < lights.size(); ++i) {
            brightness += lights[i]->findLitPoint(intersectionData);
        }
//        cout << brightness << endl;
    }

    return color * (brightness + backgroundIntensity);
}

void SceneProcessor::scanCameraMetaData() {
    FILE* cameraDataFile = freopen(cameraData.data(), "r", stdin);
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
    fclose(cameraDataFile);
}

void SceneProcessor::initKDtree() {
    Point leftBound(INFINITY,INFINITY,INFINITY);
    Point rightBound(-INFINITY,-INFINITY,-INFINITY);
    
    for(int i = 0; i < figures.size(); ++i) {
        for(int j = 0; j < 3;++j) {
            leftBound.d[j] = fmin(leftBound.d[j],figures[i]->getLeftBound(j));
            rightBound.d[j] = fmax(rightBound.d[j],figures[i]->getRightBound(j));
        }
    }
    
    kdtree = new Kdtree(figures,leftBound,rightBound);
}

void SceneProcessor::calculatePicture() {
    if(inputFileName[inputFileName.size() - 1] == 'l') {
        scanDataFromASCISTL();
    } else {
        scanDataFromMy();
    }
    
    initKDtree();

    scanCameraMetaData();
    
    picture.resize(pixelSize.first, vector <Color> (pixelSize.second));
    
    for(int i = 0;i < pixelSize.first;++i) {
        for(int j = 0;j < pixelSize.second;++j) {
            picture[i][j] = calcPixelColor(i,j);
        }
    }
    
    convertDataToFormatPPM();
}

