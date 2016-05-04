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
#define MAX_DEPTH 25

class Kdtree {
public:
    Kdtree(std::vector<std::shared_ptr<Figure> >& _data, Point& _leftBound, Point& _rightBound, int dim_out = -1, int depth = 1);
    std::tuple<Status,Point,std::shared_ptr<Figure>> find(Point& ray, Point& start);
    ~Kdtree();
private:
    int dim;
    Kdtree* leftTree;
    Kdtree* rightTree;
    Point leftBound;
    Point rightBound;
    size_t midIndex;
    ld getFirstIntersect(Point start, Point ray);
    std::vector <std::shared_ptr<Figure>>& data;
};

#endif /* kdtree_hpp */
