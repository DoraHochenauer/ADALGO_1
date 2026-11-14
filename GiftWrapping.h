//
// Created by dorish25 on 14.11.2026.
//

#ifndef ADALGO_GIFTWRAPPING_H
#define ADALGO_GIFTWRAPPING_H
#include "ConvexHull.h"


class GiftWrapping : public ConvexHull {
    private:
    Point currentPoint;
    Point favouritePoint;
    std::vector<Point> pointsInHull;


public:
    void StartAlgorithmVisual(PointCloud& point_cloud) override;
    void StartAlgorithmPerformance(PointCloud& point_cloud) override;
};


#endif //ADALGO_GIFTWRAPPING_H
