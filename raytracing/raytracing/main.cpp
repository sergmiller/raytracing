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
#define MAX_COLOR 255

using namespace std;

typedef unsigned long long ui64;

point observerPoint, controlPoint;
pair <point,point> dim;
pair <size_t,size_t> pixelSize;
vector <obj*> objs;
vector < vector<int[3]> > picture;

vector <int> getColor(int i, int j) {
    vector <int> color(3);
    point pixel = controlPoint + ((dim.first * (ld)i) + (dim.second * (ld)j));
    point ray = pixel - observerPoint;
    if(ray == point(0,0,0)) {
        return color;
    }
    
    ld firstInterDist = 1e50;
    int firstInterNumb = -1;
    
    for(int i = 0;i < objs.size(); ++i) {
        point p = objs[i]->checkIntersect(ray, observerPoint);
        
        point beam = p - observerPoint;
        
        if(beam.dist2() < firstInterDist) {
            firstInterDist = beam.dist2();
            firstInterNumb = i;
        }
    }
    
    if(firstInterNumb != -1) {
        for(int i = 0;i < 3;++i) {
            color[i] = objs[firstInterNumb]->color[i];
        }
    }
    
    return color;
}

int main(int argc, const char * argv[]) {
    freopen("input.txt","r", stdin);
    freopen("output.ppm","w", stdout);
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
            point v[3];
            for(int i = 0;i < 3;++i) {
                cin >> v[i].x >> v[i].y >> v[i].z;
            }
            objs.push_back(new triangle(color,v));
        } else {
            point c;
            ld r;
            cin >> c.x >> c.y >> c.z >> r;
            objs.push_back(new sphere(color,c,r));
        }
    }
    
    cin >> controlPoint.x >> controlPoint.y >> controlPoint.z;
    cin >> dim.first.x >> dim.first.y >> dim.first.z >> pixelSize.first;
    cin >> dim.second.x >> dim.second.y >> dim.second.z >> pixelSize.second;
    cin >> observerPoint.x >> observerPoint.y >> observerPoint.z;
    
    picture.resize(pixelSize.first, vector <int[3]> (pixelSize.second));
    
    for(int i =0;i < pixelSize.first;++i) {
        for(int j = 0;j < pixelSize.second;++j) {
            auto c = getColor(i,j);
            for(int k = 0;k < 3;++k) {
                picture[i][j][k] = c.data()[k];
            }
        }
    }
    
    return 0;
}
