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

    int _countPoints = 0;

    size_t _minXIndex = 0;
    size_t _minYIndex = 0;
    size_t _maxXIndex = 0;
    size_t _maxYIndex = 0;


public:
    void PushPoint(const Point&  point);
    //void RemovePoint(size_t index);

    int GetCountPoints() const {return _countPoints;};
    Point& GetPoint(size_t index);

    std::vector<Point> &GetPointCloud();

    void CreatePointCloud(float width, float height, float border, int numberPoints, pointcloud_mode mode);

    float GetMaxX() const;
    float GetMinX() const;



    float GetMaxY() const;
    float GetMinY() const;
    size_t GetMaxXIndex() const{return  _maxXIndex;};
    size_t GetMinXIndex() const {return  _minXIndex;};



    size_t GetMaxYIndex() const {return  _maxYIndex;};
    size_t GetMinYIndex() const {return  _minYIndex;};


    const Point& GetMaxXPoint() const;
    const Point& GetMinXPoint() const;



    const Point& GetMaxYPoint() const;
    const Point& GetMinYPoint() const;

};


#endif //ADALGO_POINTCLOUD_H
