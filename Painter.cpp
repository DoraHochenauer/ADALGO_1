//
// Created by dorish25 on 13.11.2026.
//

#include "Painter.h"

void Painter::PaintPoint(Point& point) {
    DrawCircle(point.GetPoint().x, point.GetPoint().y, 4, point.GetColor());
}

void Painter::PaintLine(const Point &pointA, const Point &pointB) {
    DrawLineEx(pointA.GetPoint(), pointB.GetPoint(), 2.0f, BLACK);
}
