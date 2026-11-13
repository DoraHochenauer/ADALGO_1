//
// Created by dorish25 on 13.11.2026.
//

#ifndef ADALGO_PAINTER_H
#define ADALGO_PAINTER_H
#include "Point.h"


class Painter {
public:
    void PaintPoint(Point &point);

    // progress 0..1: wie viel der Linie von A Richtung B gezeichnet wird (1 = ganz)
    void PaintLine(const Point &pointA, const Point &pointB, float progress = 1.0f);
};


#endif //ADALGO_PAINTER_H
