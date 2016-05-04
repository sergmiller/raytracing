//
//  kdtree.cpp
//  raytracing
//
//  Created by Сергей Миллер on 04.05.16.
//  Copyright © 2016 sergmiller. All rights reserved.
//

#include "kdtree.hpp"
#include <algorithm>

using std::max;
using std::vector;
using std::shared_ptr;

struct cmpWithDim {
    int dim;
    cmpWithDim(int _dim) { dim = _dim; }
    bool operator() (shared_ptr<Figure> f1,shared_ptr<Figure> f2) {
        return f1->getRightBound(dim) < f2->getRightBound(dim);
    }
};

Kdtree::~Kdtree() {
    if(leftTree) {
        delete leftTree;
    }
    
    if(rightTree) {
        delete rightTree;
    }
}

Kdtree::Kdtree(vector<shared_ptr<Figure> >& _data, Point& _leftBound, Point& _rightBound , int dim_out, int depth): data(_data), leftBound(_leftBound), rightBound(_rightBound) {
    dim = (dim_out + 1)%3;
    
    size_t size = data.size();
    
    if(depth > MAX_DEPTH || !size) {
        leftTree = nullptr;
        rightTree = nullptr;
        return;
    }
    
    cmpWithDim cmp(dim);
    sort(data.begin(),data.end(),cmp);
    
    ld median = data[size/2]->getRightBound(dim);
    
    midIndex = size/2;
    
//    ld defaultSAH = EMPTY_COST + getSurface(leftBound,rightBound) * size;
//    
//    int indexMinSAH = -1;
//    ld minSAH = 0;
//    ld coordsMidForLeft[3] = {rightBound[0], rightBound[1], rightBound[2]};
//    ld coordsMidForRight[3] = {leftBound[0], leftBound[1], leftBound[2]};
////    ld halfDimLen = (rightBound[dim] - leftBound[dim])/2;
//
//    for(int i = 0;i < size; ++i) {
//        coordsMidForRight[dim]  = coordsMidForLeft[dim] = data[i]->getRightBound(dim);
//        Point boundForLeft(coordsMidForLeft), boundForRight(coordsMidForRight);
//        ld SAH = EMPTY_COST + getSurface(leftBound, boundForLeft) * i + getSurface(boundForRight, rightBound) * (size - i) * 1.5;
//        if(SAH < minSAH || indexMinSAH == -1) {
//            indexMinSAH = i;
//            minSAH = SAH;
//        }
//    }
//    
//    if(minSAH > defaultSAH - EPS) {
//        leftTree = nullptr;
//        rightTree = nullptr;
//    }
    
    
    vector <shared_ptr<Figure> > dataLeft;
    vector <shared_ptr<Figure> > dataRight;
    
    for(int i = 0; i < size;++i) {
        if(data[i]->getLeftBound(dim) < median) {
            dataLeft.push_back(data[i]);
        }
        
        if(data[i]->getRightBound(dim) >= median) {
            dataRight.push_back(data[i]);
        }
    }
    
    ld halfDimLen = (rightBound[dim] - leftBound[dim])/2;
    
    ld dir[3] = {0,0,0};
    dir[dim] += halfDimLen;
    Point offset(dir);
    Point boundForLeft = rightBound - offset;
    Point boundForRight = leftBound + offset;
    
    leftTree = new Kdtree(dataLeft,leftBound,boundForLeft,dim,depth+1);
    rightTree = new Kdtree(dataRight,boundForRight,rightBound,dim,depth+1);
}

std::tuple<Status,Point,std::shared_ptr<Figure>> Kdtree::find(Point& ray, Point& start) {
    Point intersection;
    std::shared_ptr<Figure> intersectFigure = nullptr;
    ld intersectDist2 = INFINITY;
    Status intersectionStatus = NOT_INTERSECT;

    for(std::shared_ptr<Figure> figure : data) {
        pair <Status, Point> intersectData = figure->checkIntersect(ray, start);
        if(intersectData.first == NOT_INTERSECT) {
            continue;
        }
        
        Point vecToCurrentIntersection = intersectData.second - start;
        
        ld dist2ToCurrentIntersection = vecToCurrentIntersection.dist2();
        
        if(dist2ToCurrentIntersection < intersectDist2) {
            intersectionStatus = intersectData.first;
            intersectDist2 = dist2ToCurrentIntersection;
            intersection = intersectData.second;
            intersectFigure = figure;
        }
    }

    return std::make_tuple(intersectionStatus, intersection, intersectFigure);
}

