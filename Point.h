//
// Created by dorish25 on 13.11.2026.
//

#ifndef ADALGO_POINT_H
#define ADALGO_POINT_H
#include "raylib.h"


class Point {

    private:
    Vector2 _point;
    Color _color;



    public:
    Point(Vector2 point, Color color = BLUE) : _point(point), _color(color) {};


    Vector2 GetPoint() const { return _point; }
    Color GetColor() const { return _color; }


    void SetColor(Color color) { _color = color; }
};


#endif //ADALGO_POINT_H
