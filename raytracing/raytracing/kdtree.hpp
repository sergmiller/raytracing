//
//  kdtree.hpp
//  raytracing
//
//  Created by Сергей Миллер on 04.05.16.
//  Copyright © 2016 sergmiller. All rights reserved.
//

#ifndef kdtree_hpp
#define kdtree_hpp

#include <stdio.h>
#include <vector>
#include "geometrystructs.hpp"

#define EMPTY_COST 0
#define MAX_DEPTH 15

class Kdtree {
public:
    Kdtree(std::vector<std::shared_ptr<Figure> >& _data, Point& _leftBound, Point& _rightBound, int dim_out = -1, int depth = 1, int notDiv = 0);
    void find(Point& ray, Point& start, ld& bestDist, IntersectionData& bestIntersection, std::pair<ld,ld>  boundingBoxIntersection);
    IntersectionData find(Point& ray, Point& start);
    bool pointInside(Point& point);
    ~Kdtree();
private:
    int dim;
    Kdtree* leftTree;
    Kdtree* rightTree;
    Point leftBound;
    Point rightBound;
    ld median;
    size_t cutIndex;
    std::pair<ld,ld> getIntersectionRatioWithBoundingBox(Point& ray, Point& start);
    std::vector <std::shared_ptr<Figure>> data;
    ld getIntersectionRatioWithMedianFlat(Point& ray, Point& start);
};

#endif /* kdtree_hpp */
