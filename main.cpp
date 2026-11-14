#include <iostream>
#include "raylib.h"
#include <vector>

#include "GiftWrapping.h"
#include "Painter.h"
#include "PointCloud.h"
#include "raymath.h"


const pointcloud_mode selectedMode = pointcloud_mode::random_points;

const program_mode programMode = program_mode::visual;
const int numberPoints = 20;

const int winWidth = 800;
const int winHeight = 600;

const int drawBorder = 20;


int main() {
    PointCloud points;
    Painter painter;

    float progress = 0.0f;
    const float secondsPerLine = 0.5f;

    points.CreatePointCloud(winWidth, winHeight, drawBorder, numberPoints, selectedMode);

    if (programMode == program_mode::visual) {
        InitWindow(winWidth, winHeight, "ADALGO - TASK1");
        SetTargetFPS(60);

        GiftWrapping gw;
       gw.StartAlgorithmVisual(points);

        while (!WindowShouldClose()) {
            progress += GetFrameTime() / secondsPerLine;

            BeginDrawing();
            ClearBackground(RAYWHITE);

            for (Point &p: points.GetPointCloud()) {
                painter.PaintPoint(p);
            }



            /*
            // Linie i wächst, während progress von i auf i+1 steigt (PaintLine begrenzt auf 0..1)
            painter.PaintLine(points.GetMinXPoint(), points.GetMaxYPoint(), progress - 0);
            painter.PaintLine(points.GetMaxYPoint(), points.GetMaxXPoint(), progress - 1);
            painter.PaintLine(points.GetMaxXPoint(), points.GetMinYPoint(), progress - 2);
            painter.PaintLine(points.GetMinYPoint(), points.GetMinXPoint(), progress - 3);
*/
            EndDrawing();
        }

        CloseWindow();
    }
    return 0;
}
