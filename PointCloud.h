//
// Created by dorish25 on 12.11.2026.
//
#ifndef ADALGO_POINTCLOUD_H
#define ADALGO_POINTCLOUD_H

#include "raylib.h"
#include <vector>
#include <cmath>
#include "enums.h"
#include "Point.h"

class PointCloud {
private:
    std::vector<Point> _pointCloud;


    size_t _minXIndex = 0;
    size_t _minYIndex = 0;
    size_t _maxXIndex = 0;
    size_t _maxYIndex = 0;


    void PushPoint(const Point&  point);



public:
    std::vector<Point> &GetPointCloud();

    void CreatePointCloud(float width, float height, float border, int numberPoints, mode mode);

    float GetMaxX() const;
    float GetMinX() const;



    float GetMaxY() const;
    float GetMinY() const;


    const Point& GetMaxXPoint() const;
    const Point& GetMinXPoint() const;



    const Point& GetMaxYPoint() const;
    const Point& GetMinYPoint() const;

};


#endif //ADALGO_POINTCLOUD_H
