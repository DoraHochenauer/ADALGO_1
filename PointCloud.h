//
// Created by dorish25 on 12.11.2026.
//
#ifndef ADALGO_POINTCLOUD_H
#define ADALGO_POINTCLOUD_H

#include "raylib.h"
#include <vector>
#include "enums.h"

class PointCloud {
private:
    std::vector<Vector2> _pointCloud;

public:
    std::vector<Vector2>& GetPointCloud() ;

    void CreatePointCloud(float width, float height, float border, int numberPoints, mode mode);
};


#endif //ADALGO_POINTCLOUD_H
