//
// Created by dorish25 on 12.11.2026.
//

#include "PointCloud.h"
#include "RandomGenerator.h"

#include <cmath>
#include <algorithm>

std::vector<Vector2>& PointCloud::GetPointCloud() {
    return _pointCloud;
}

void PointCloud::CreatePointCloud(float width, float height, float border, int numberPoints, mode mode) {
    _pointCloud.clear();
    RandomGenerator rg;

    switch (mode) {

        case mode::line:
            for (int i = 0; i < numberPoints; i++) {
                _pointCloud.push_back(Vector2{
                    rg.randomFloat(border, width - border), height / 2.0f
                });
            }
            break;
        case mode::random_points:
            for (int i = 0; i < numberPoints; i++) {
                _pointCloud.push_back(Vector2{
                    rg.randomFloat(border, width - border), rg.randomFloat(border, height - border)
                });
            }
            break;

        case mode::circle: {
            float cx = width / 2.0f;
            float cy = height / 2.0f;
            float radius  = std::min(width, height) / 2.0f - border;
            for (int i = 0; i < numberPoints; i++) {
                float angle = 2.0f * PI * (float)i / (float)numberPoints;
                _pointCloud.push_back(Vector2{
                    cx + radius * std::cos(angle), cy + radius * std::sin(angle)
                });
            }
            break;
        }
        default: break;
    }



}
