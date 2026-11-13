//
// Created by dorish25 on 13.11.2026.
//

#ifndef ADALGO_PAINTER_H
#define ADALGO_PAINTER_H
#include "Point.h"


class Painter {
public:
    void PaintPoint(Point &point);

    void PaintLine(const Point &pointA, const Point &pointB);
};


#endif //ADALGO_PAINTER_H
