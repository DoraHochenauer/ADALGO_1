//
// Created by dorish25 on 13.11.2026.
//

#ifndef ADALGO_PAINTER_H
#define ADALGO_PAINTER_H
#include "Point.h"
#include "GiftWrapping.h"


class Painter {
public:
    void PaintPoint(Point &point);
    void PaintLine(const Point &pointA, const Point &pointB, float progress = 1.0f, Color color = BLACK);

    // gestrichelte Linie: abwechselnd dashLength Pixel Strich und gapLength Pixel Lücke
    void PaintDashedLine(const Point &pointA, const Point &pointB, Color color = GRAY,
                         float dashLength = 10.0f, float gapLength = 6.0f);


    void PaintGiftWrapper(GiftWrapping& gw, PointCloud& point, int winHeight);
};


#endif //ADALGO_PAINTER_H
