//
// Created by dorish25 on 12.11.2026.
//

#include "PointCloud.h"
#include "RandomGenerator.h"

#include <cmath>
#include <algorithm>
#include <iostream>
#include <ostream>

std::vector<Point> &PointCloud::GetPointCloud() {
    return _pointCloud;
}


float PointCloud::GetMaxX() const {
    return GetMaxXPoint().GetPoint().x;
}

float PointCloud::GetMinX() const {
    return GetMinXPoint().GetPoint().x;
}

float PointCloud::GetMaxY() const {
    return GetMaxYPoint().GetPoint().y;
}

float PointCloud::GetMinY() const {
    return GetMinYPoint().GetPoint().y;
}

const Point& PointCloud::GetMaxXPoint() const {
    return _pointCloud[_maxXIndex];
}

const Point& PointCloud::GetMinXPoint() const {
    return _pointCloud[_minXIndex];
}

const Point& PointCloud::GetMaxYPoint() const {
    return _pointCloud[_maxYIndex];
}

const Point& PointCloud::GetMinYPoint() const {
    return _pointCloud[_minYIndex];
}

void PointCloud::PushPoint(const Point &point) {
    _pointCloud.push_back(point);
    size_t i = _pointCloud.size() - 1;
    Vector2 p = point.GetPoint();


    if (p.x < _pointCloud[_minXIndex].GetPoint().x) _minXIndex = i;
    if (p.x > _pointCloud[_maxXIndex].GetPoint().x) _maxXIndex = i;
    if (p.y < _pointCloud[_minYIndex].GetPoint().y) _minYIndex = i;
    if (p.y > _pointCloud[_maxYIndex].GetPoint().y) _maxYIndex = i;

    std::cout << "POINT: " << p.x << " " << p.y << std::endl;
}



void PointCloud::CreatePointCloud(float width, float height, float border, int numberPoints, mode mode) {
    _pointCloud.clear();
    _minXIndex = _maxXIndex = _minYIndex = _maxYIndex = 0;

    RandomGenerator rg;

    switch (mode) {
        case mode::line:
            for (int i = 0; i < numberPoints; i++) {
                PushPoint(Vector2{
                    rg.randomFloat(border, width - border), height / 2.0f
                });
            }
            break;
        case mode::random_points:
            for (int i = 0; i < numberPoints; i++) {
                PushPoint(Vector2{
                    rg.randomFloat(border, width - border), rg.randomFloat(border, height - border)
                });
            }
            break;

        case mode::circle: {
            float cx = width / 2.0f;
            float cy = height / 2.0f;
            float radius = std::min(width, height) / 2.0f - border;
            for (int i = 0; i < numberPoints; i++) {
                float angle = 2.0f * PI * (float) i / (float) numberPoints;
                PushPoint(Vector2{
                    cx + radius * std::cos(angle), cy + radius * std::sin(angle)
                });
            }
            break;
        }
        default: break;
    }


    if (_pointCloud.empty()) {
        return;
    }

    // Extremwerte sind rot
    _pointCloud[_maxXIndex].SetColor(RED);
    _pointCloud[_maxYIndex].SetColor(RED);
    _pointCloud[_minXIndex].SetColor(RED);
    _pointCloud[_minYIndex].SetColor(RED);

    std::cout << "MAX X: " << GetMaxXPoint().GetPoint().x << " " << GetMaxXPoint().GetPoint().y << std::endl;
    std::cout << "MAX Y: " << GetMaxYPoint().GetPoint().x << " " << GetMaxYPoint().GetPoint().y << std::endl;
    std::cout << "MIN X: " << GetMinXPoint().GetPoint().x << " " << GetMinXPoint().GetPoint().y << std::endl;
    std::cout << "MIN Y: " << GetMinYPoint().GetPoint().x << " " << GetMinYPoint().GetPoint().y << std::endl;
}
