//
// Created by dorish25 on 12.11.2026.
//

#include "PointCloud.h"
#include "RandomGenerator.h"

#include <cmath>
#include <algorithm>
#include <iostream>
#include <ostream>
#include <fstream>

 Point& PointCloud::GetPoint(size_t index) {
     return _pointCloud[index];
 }

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
    _countPoints ++;
    _pointCloud.push_back(point);
    size_t i = _pointCloud.size() - 1;
    Vector2 p = point.GetPoint();


    if (p.x < _pointCloud[_minXIndex].GetPoint().x) _minXIndex = i;
    if (p.x > _pointCloud[_maxXIndex].GetPoint().x) _maxXIndex = i;
    if (p.y < _pointCloud[_minYIndex].GetPoint().y) _minYIndex = i;
    if (p.y > _pointCloud[_maxYIndex].GetPoint().y) _maxYIndex = i;

}



void PointCloud::CreatePointCloud(float width, float height, float border, int numberPoints, pointcloud_mode mode) {
    _pointCloud.clear();
    _minXIndex = _maxXIndex = _minYIndex = _maxYIndex =_countPoints = 0;

    RandomGenerator rg;

    switch (mode) {
        case pointcloud_mode::line:
            for (int i = 0; i < numberPoints; i++) {
                PushPoint(Vector2{
                    rg.randomFloat(border, width - border), height / 2.0f
                });
            }
            break;
        case pointcloud_mode::random_points:
            for (int i = 0; i < numberPoints; i++) {
                PushPoint(Vector2{
                    rg.randomFloat(border, width - border), rg.randomFloat(border, height - border)
                });
            }
            break;

        case pointcloud_mode::circle: {
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


    ColorExtremes();
}

void PointCloud::ColorExtremes() {
    if (_pointCloud.empty()) {
        return;
    }

    // Extremwerte sind rot
    _pointCloud[_maxXIndex].SetColor(RED);
    _pointCloud[_maxYIndex].SetColor(RED);
    _pointCloud[_minXIndex].SetColor(RED);
    _pointCloud[_minYIndex].SetColor(RED);
}

bool PointCloud::CreatePointCloudFile(const std::string& filename) {
    _pointCloud.clear();
    _minXIndex = _maxXIndex = _minYIndex = _maxYIndex = _countPoints = 0;

    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Datei nicht gefunden: " << filename << std::endl;
        return false;
    }

    // erste Zeile: Anzahl der Punkte
    int count = 0;
    if (!(file >> count) || count < 0) {
        std::cerr << "Erste Zeile ist keine gueltige Anzahl: " << filename << std::endl;
        return false;
    }
    _pointCloud.reserve(count);

    // danach pro Punkt "x,y"
    // >> überspringt Leerzeichen, Leerzeilen und Windows-Zeilenenden (\r\n) automatisch
    for (int i = 0; i < count; i++) {
        float x, y;
        char comma;
        if (!(file >> x >> comma >> y) || comma != ',') {
            std::cerr << "Fehler beim Lesen von Punkt " << i + 1 << " von " << count
                      << " (erwartet: x,y)" << std::endl;
            break;   // bis hierher gelesene Punkte behalten
        }
        PushPoint(Vector2{x, y});
    }

    ColorExtremes();
    return !_pointCloud.empty();
}
