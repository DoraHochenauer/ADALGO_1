#include <iostream>
#include "raylib.h"
#include <vector>

#include "Painter.h"
#include "PointCloud.h"
#include "raymath.h"


const mode selected_mode = mode::random_points;
const int numberPoints = 20;

const int winWidth = 800;
const int winHeight = 600;

const int drawBorder = 20;



int main() {
    // create points
    PointCloud points;
    Painter painter;
    float progress = 0.0f;
    const float secondsPerLine = 0.5f;

    points.CreatePointCloud(winWidth, winHeight, drawBorder, numberPoints, selected_mode);

    InitWindow(winWidth, winHeight, "ADALGO - TASK1");
    SetTargetFPS(60);


    //DrawCall
    while (!WindowShouldClose()) {
        progress += GetFrameTime() / secondsPerLine;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (Point& p : points.GetPointCloud()) {
            painter.PaintPoint(p);
        }

        // Linie i wächst, während progress von i auf i+1 steigt (PaintLine begrenzt auf 0..1)
        painter.PaintLine(points.GetMinXPoint(), points.GetMaxYPoint(), progress - 0);
        painter.PaintLine(points.GetMaxYPoint(), points.GetMaxXPoint(), progress - 1);
        painter.PaintLine(points.GetMaxXPoint(), points.GetMinYPoint(), progress - 2);
        painter.PaintLine(points.GetMinYPoint(), points.GetMinXPoint(), progress - 3);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
