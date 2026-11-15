//
// Created by dorish25 on 13.11.2026.
//

#include "Painter.h"
#include "raymath.h"
#include <algorithm>

void Painter::PaintPoint(Point& point) {
    DrawCircle(point.GetPoint().x, point.GetPoint().y, 4, point.GetColor());
}

void Painter::PaintLine(const Point &pointA, const Point &pointB, float progress, Color color) {
    if (progress <= 0.0f) return;

    Vector2 a = pointA.GetPoint();
    Vector2 b = pointB.GetPoint();
    DrawLineEx(a, Vector2Lerp(a, b, Clamp(progress, 0.0f, 1.0f)), 2.0f, color);
}

void Painter::PaintDashedLine(const Point &pointA, const Point &pointB, Color color,
                              float dashLength, float gapLength) {
    Vector2 a = pointA.GetPoint();
    Vector2 b = pointB.GetPoint();

    float length = Vector2Distance(a, b);
    if (length <= 0.0f) return;   // gleicher Punkt -> nichts zu zeichnen

    Vector2 direction = Vector2Scale(Vector2Subtract(b, a), 1.0f / length);   // Länge 1

    // entlang der Linie: Strich zeichnen, Lücke überspringen, ...
    for (float start = 0.0f; start < length; start += dashLength + gapLength) {
        float end = std::min(start + dashLength, length);   // letzter Strich nicht über b hinaus
        DrawLineEx(Vector2Add(a, Vector2Scale(direction, start)),
                   Vector2Add(a, Vector2Scale(direction, end)),
                   2.0f, color);
    }
}

void Painter::PaintGiftWrapper(GiftWrapping& gw, PointCloud& points, int winHeight) {

        DrawText("Pfeil rechts = weiter", 10, 10, 20, DARKGRAY);
        Point &currentPoint = points.GetPoint(gw.GetCurrent());
        if (gw.GetState() == giftwrapp_state::find_best_favourite) {
            if (gw.GetBestFavourtite() < points.GetCountPoints()) {
                PaintDashedLine(currentPoint, points.GetPoint(gw.GetBestFavourtite()), LIGHTGRAY);
            }
        }
        if (gw.GetState() == giftwrapp_state::find_best_favourite ||
            gw.GetState() == giftwrapp_state::add_favourite) {
            // orange: aktueller Favorit
            PaintDashedLine(currentPoint, points.GetPoint(gw.GetFavourite()), ORANGE);
            }

        for (Point &p: points.GetPointCloud()) {
            PaintPoint(p);
        }
        std::vector<Point> &hull = gw.GetHull().GetPointCloud();
        for (size_t i = 0; i + 1 < hull.size(); i++) {
            PaintLine(hull[i], hull[i + 1]);
        }

        DrawText(TextFormat("Zustand: %s", gw.GetStateName()), 10, winHeight - 30, 20, DARKGRAY);

}
