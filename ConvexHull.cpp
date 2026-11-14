//
// Created by dorish25 on 14.11.2026.
//

#include <cmath>
#include "ConvexHull.h"

#include "Point.h"

// Achtung Raylib Vector zählt nach unten! + - umgekehrt werten
float ConvexHull::CrossProduct(const Vector2& la, const Vector2& lb, const Vector2& c) {
    return (lb.x - la.x) * (c.y - la.y) - (lb.y - la.y) * (c.x - la.x);
}

float ConvexHull::Distance(const Vector2& a, const Vector2& b) {
    return std::hypot(a.x - b.x, a.y - b.y);
}

void ConvexHull::AddPoint(Point& point) {
    _pointsInHull.PushPoint(point);
    point.SetColor(GREEN);

}