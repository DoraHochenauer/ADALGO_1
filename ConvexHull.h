//
// Created by dorish25 on 14.11.2026.
//

#ifndef ADALGO_CONVEXHULL_H
#define ADALGO_CONVEXHULL_H
#include <vector>

#include "PointCloud.h"
#include "raylib.h"


class Point;
class PointCloud;

// Abstrakte Basisklasse: jeder Hull-Algorithmus (GiftWrapping, QuickHull, ...) erbt davon
class ConvexHull {
protected:
    // protected: nur ConvexHull und seine Unterklassen dürfen darauf zugreifen
    PointCloud _pointsInHull;

    void AddPoint(Point& point);

public:
    virtual ~ConvexHull() = default;

    // Hüllpunkte in der Reihenfolge, in der der Algorithmus sie gefunden hat
    PointCloud& GetHull() { return _pointsInHull; }

    static float CrossProduct(const Vector2& la, const Vector2& lb, const Vector2& c);
    static float Distance(const Vector2& a, const Vector2& b);


    virtual void StartAlgorithmVisual(PointCloud& point_cloud) = 0;
    virtual void StartAlgorithmPerformance(PointCloud& point_cloud) = 0;
    virtual void Init(PointCloud &point_cloud) = 0;
    virtual void Step() = 0;
};


#endif //ADALGO_CONVEXHULL_H
