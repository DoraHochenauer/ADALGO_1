//
// Created by dorish25 on 13.11.2026.
//

#include "Painter.h"
#include "raymath.h"

void Painter::PaintPoint(Point& point) {
    DrawCircle(point.GetPoint().x, point.GetPoint().y, 4, point.GetColor());
}

void Painter::PaintLine(const Point &pointA, const Point &pointB, float progress) {
    if (progress <= 0.0f) return;   // Linie ist noch nicht dran

    Vector2 a = pointA.GetPoint();
    Vector2 b = pointB.GetPoint();
    DrawLineEx(a, Vector2Lerp(a, b, Clamp(progress, 0.0f, 1.0f)), 2.0f, BLACK);
}
