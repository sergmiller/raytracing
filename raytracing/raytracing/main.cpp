//
//  main.cpp
//  raytracing
//
//  Created by Сергей Миллер on 10.04.16.
//  Copyright © 2016 sergmiller. All rights reserved.
//

#include <iostream>
#include <vector>

#include "raytracingstructs.hpp"
#include "lighting.hpp"
#define MAX_COLOR 255
#define INPUT "input.txt" 
#define OUTPUT "output.ppm"

using namespace std;

typedef unsigned long long ui64;

Point observerPoint, controlPoint;
pair <Point,Point> dim;
pair <size_t,size_t> pixelSize;
vector <Figure*> figures;
vector <vector<Color> > picture;
vector <LightSource> lights;

Color getColor(int x, int y) {
    Color color;
    Point pixel = controlPoint + ((dim.first * (ld)x) + (dim.second * (ld)y));

    Point ray = pixel - observerPoint;
    if(ray == Point(0,0,0)) {
        return color;
    }
    
    ld firstInterDist = 1e50;
    int firstInterNumb = -1;
    
    for(int i = 0;i < figures.size(); ++i) {
        std::pair<status,Point> res = figures[i]->checkIntersect(ray, observerPoint);

        if(res.first == NOT_INTERSECT) {
            continue;
        }
        
        Point p = res.second;
        
        Point beam = p - observerPoint;
        
        if(beam.dist2() < firstInterDist) {
            firstInterDist = beam.dist2();
            firstInterNumb = i;
        }
    }
    
    if(firstInterNumb != -1) {
        color = figures[firstInterNumb]->color;
    }
    
    if(
    ld intensity = 0;
    for(int i = 0;i < lights.size(); ++i) {
        intensity += lights[i].calcIntensityForPoint(intersect);
    }
    
    return color;
}

void convertDataToFormatPPM() {
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

void scanData() {
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
    
    cin >> controlPoint.x >> controlPoint.y >> controlPoint.z;
    cin >> dim.first.x >> dim.first.y >> dim.first.z >> pixelSize.first;
    cin >> dim.second.x >> dim.second.y >> dim.second.z >> pixelSize.second;
    cin >> observerPoint.x >> observerPoint.y >> observerPoint.z;
    

}

int main(int argc, const char * argv[]) {
    freopen(INPUT,"r", stdin);
    freopen(OUTPUT,"w", stdout);
    scanData();
        picture.resize(pixelSize.first, vector <Color> (pixelSize.second));
    
    for(int i =0;i < pixelSize.first;++i) {
        for(int j = 0;j < pixelSize.second;++j) {
            picture[i][j] = getColor(i,j);
        }
    }
    
    convertDataToFormatPPM();
    
    return 0;
}
