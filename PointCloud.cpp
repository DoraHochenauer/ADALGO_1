//
// Created by dorish25 on 12.11.2026.
//

#include "PointCloud.h"
#include "RandomGenerator.h"

std::vector<Vector2>& PointCloud::GetPointCloud() {
    return _pointCloud;
}

void PointCloud::CreatePointCloud(float width, float height, float border, int numberPoints, mode mode) {
    RandomGenerator* rg = new RandomGenerator();

    switch (mode) {

        case mode::line:
            for (int i = 0; i < numberPoints; i++) {
                _pointCloud.push_back(Vector2{
                    rg->randomFloat(border, width - border), height / 2.0f
                });
            }
            break;
        case mode::random_points:
            for (int i = 0; i < numberPoints; i++) {
                _pointCloud.push_back(Vector2{
                    rg->randomFloat(border, width - border), rg->randomFloat(border, height - border)
                });
            }
            break;
        default: break;
    }



    delete rg;
}
