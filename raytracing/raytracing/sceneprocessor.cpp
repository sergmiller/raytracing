//
//  sceneprocessor.cpp
//  raytracing
//
//  Created by Сергей Миллер on 17.04.16.
//  Copyright © 2016 sergmiller. All rights reserved.
//

#include "sceneprocessor.hpp"

using std::fmax;

SceneProcessor::SceneProcessor(ld intensity):backgroundIntensity(intensity), initCameraData(false), leftBoundScene(Point(INFINITY,INFINITY,INFINITY)), rightBoundScene(Point(-INFINITY,-INFINITY,-INFINITY)) {};

SceneProcessor& SceneProcessor::scanDataFromASCISTL(string input) {
    FILE* in = freopen(input.data(), "r", stdin);
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
    
    fclose(in);
    return *this;
}

SceneProcessor& SceneProcessor::scanDataFromMy(string input) {
    FILE* in = freopen(input.data(), "r", stdin);
    int n;
    cin >> n;
    for (int i = 0;i < n;++i) {
        int type;
        int color[3];
        int refl;
        cin >> type;
        for(int i = 0;i < 3;++i) {
            cin >> color[i];
            color[i]  = std::min(255,std::max(1,color[i]));
        }
        cin >> refl;
        
        if(refl > 100 || refl < 0) {
            refl = 0;
        }
        
        if(!type) {
            Point v[3];
            for(int i = 0;i < 3;++i) {
                cin >> v[i].x >> v[i].y >> v[i].z;
            }
            figures.push_back(std::shared_ptr<Figure>(new Triangle(Color(color),v,Point(0,0,0),refl)));
        } else {
            Point c;
            ld r;
            cin >> c.x >> c.y >> c.z >> r;
            figures.push_back(std::shared_ptr<Figure>(new Sphere(Color(color),c,r,refl)));
        }
    }
    
    fclose(in);
    return *this;
}

SceneProcessor& SceneProcessor::printDataWithFormatPPM(string output) {
    FILE* out = freopen(output.data(), "w", stdout);
    cout << "P3\n";
    cout << pixelSize.second << " " <<  pixelSize.first << "\n";
    cout << MAX_COLOR << "\n";
    for(int i = 0;i < pixelSize.first;++i) {
        for(int j = 0; j < pixelSize.second; ++j) {
            cout << picture[i][j].R << " " << picture[i][j].G << " " << picture[i][j].B;
            cout << "\n";
        }
    }
    fclose(out);
    return *this;
}

Color SceneProcessor::calcColorInPoint(Point ray, Point start, int contribution, int depth) {
    Color color;
    if(contribution < 5 || depth > 5 || ray == Point(0,0,0)) {
        return color;
    }
    
    IntersectionData intersectionData = kdtree->find(ray, start);
    
    ld brightness = 0;
    
    if(status(intersectionData) != NOT_INTERSECT) {
        ld textureAlpha = figure(intersectionData)->getTextureAlpha();
        
        Color textureColor;
        
        int textureId = figure(intersectionData)->getTextureId();
        
        if(textureId != -1) {
            textureColor =  figure(intersectionData)->calcTextureColor(point(intersectionData), textures[textureId]);
        }
        
        color = figure(intersectionData)->getColor() * (1 - (textureAlpha/100)) + textureColor * (textureAlpha/100);
        for(int i = 0;i < lights.size(); ++i) {
            brightness += lights[i].findLitPoint(intersectionData, kdtree);
        }
        ld reflectAlpha = (ld)figure(intersectionData)->getReflectAlpha();
        Point reflectedRay = getReflectionRay(ray, figure(intersectionData)->getFrontSideNormalInPoint(point(intersectionData)));
        Color reflectColor = calcColorInPoint(reflectedRay, point(intersectionData) + reflectedRay * (EPS*EPS), contribution * (reflectAlpha - 1)/100, ++depth);
        
        color = color * (brightness + backgroundIntensity) * (1 - (reflectAlpha/100)) + reflectColor * (reflectAlpha/100);
    }
    
    return color;
}

Color SceneProcessor::calcPixel(int _x, int _y) {
    Color color;
    Point pixel = controlPoint + ((dim.first * (ld)_x) + (dim.second * (ld)_y));
    Point ray = pixel - observerPoint;
    
    return calcColorInPoint(ray, pixel, 100, 1);
}

SceneProcessor& SceneProcessor::scanLightData(string light) {
    FILE* in = freopen(light.data(), "r", stdin);
    int k;
    cin >> k;
    for(int i = 0;i < k;++i) {
        Point c;
        ld intense;
        cin >> c.x >> c.y >> c.z >> intense;
        lights.push_back(LightSource(intense,c));
    }
    fclose(in);
    return *this;
}

SceneProcessor& SceneProcessor::scanCameraData(string camera) {
    FILE* in = freopen(camera.data(), "r", stdin);
    cin >> controlPoint.x >> controlPoint.y >> controlPoint.z;
    cin >> dim.first.x >> dim.first.y >> dim.first.z >> pixelSize.first;
    cin >> dim.second.x >> dim.second.y >> dim.second.z >> pixelSize.second;
    cin >> observerPoint.x >> observerPoint.y >> observerPoint.z;
    fclose(in);
    initCameraData = true;
    return *this;
}

void SceneProcessor::initKDtree() {

    for(int i = 0; i < figures.size(); ++i) {
        for(int j = 0; j < 3;++j) {
            leftBoundScene.d[j] = fmin(leftBoundScene.d[j],figures[i]->getLeftBound(j));
            rightBoundScene.d[j] = fmax(rightBoundScene.d[j],figures[i]->getRightBound(j));
        }
    }
    
    kdtree = std::shared_ptr<Kdtree>(new Kdtree(figures,leftBoundScene,rightBoundScene));
}

SceneProcessor& SceneProcessor::run() {
    initKDtree();
    if(!initCameraData) {
        autoCameraPosition();
    }
    cout << "size: " << figures.size() << endl;
    cout << "pixelsize: " << pixelSize.first << " " << pixelSize.second << endl;
    picture.resize(pixelSize.first, vector <Color> (pixelSize.second));
    
    for(int i = 0;i < pixelSize.first;++i) {
        for(int j = 0;j < pixelSize.second;++j) {
            picture[i][j] = calcPixel(i,j);
        }
    }
    
    return *this;
}

void SceneProcessor::autoCameraPosition() {
    Point centr = (leftBoundScene + rightBoundScene)*(0.5);
    Point right = rightBoundScene - centr;
    Point left = leftBoundScene - centr;
    right.printPoint();
    left.printPoint();
    ld maxdev = fmax(right.x, fmax(right.y, right.z));
    maxdev =  fmax(fmax(maxdev,-left.x), fmax(-left.y, -left.z));
    maxdev *= 20;
    cout << maxdev << endl;
    
    Point normalToScreen = Point(-maxdev,maxdev*2,maxdev/5);
    
    Point centrScreen = centr + normalToScreen;
    
    ld len = maxdev*sqrtl(2.0)/(4 * YDIM);
    
    dim.second = Point(3/sqrtl(10.0), 1/sqrtl(10.0), 0) * len;
    Point vectx = vect(dim.second, normalToScreen);
    if(vectx.z > 0) {
        vectx = vectx * (-1);
    }
    
    dim.first = vectx * (1/sqrtl(vectx.dist2())) * len;
    
    controlPoint = centrScreen - (dim.first * (XDIM/2)) - (dim.second * (YDIM/2));
    observerPoint = centr + (normalToScreen * (1/sqrtl(normalToScreen.dist2()))) * 1e8;
    dim.first.printPoint();
    dim.second.printPoint();
    observerPoint.printPoint();
    controlPoint.printPoint();
    centrScreen.printPoint();
    pixelSize = std::make_pair(XDIM,YDIM);
    cout << maxdev << endl;
    
    initCameraData = true;
}

SceneProcessor& SceneProcessor::loadTextureFromPPMWithKey(string name, string key) {
    FILE* in = freopen(name.data(), "r", stdin);
    string s;
    cin >> s;
    if(s != "P6") {
        throw new std::bad_function_call;
    }
    int len, high;
    cin >> len >> high;
    cout << len <<" " <<  high << endl;
    int maxcolor;
    cin >> maxcolor;
    texturesId[key] = (int)textures.size();
    textures.push_back(Picture(high,vector <Color>(len)));
    Picture& currentTexture = textures[textures.size() - 1];
    for(int i = 0; i < high; ++i) {
        for(int j = 0; j < len;++j) {
            currentTexture[i][j].R = (int)getchar();
            currentTexture[i][j].G = (int)getchar();
            currentTexture[i][j].B = (int)getchar();
        }
    }
    
    fclose(in);
    
    return *this;
}

SceneProcessor& SceneProcessor::addTextureMap(string texturemap) {
    FILE* in = freopen(texturemap.data(), "r", stdin);
    int k, numb, alpha;
    string textureName;
    cin >> k;
    for(int i = 0;i < k;++i) {
        cin >> numb >> textureName >> alpha;
        --numb;
        if(alpha > 0 && alpha <= 100 && numb >= 0 && numb < figures.size() && texturesId[textureName] > 0) {
            figures[numb]->setTexture(texturesId[textureName],alpha);
        }
    }
    
    fclose(in);
    return *this;
}



