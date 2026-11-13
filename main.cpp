#include <iostream>
#include "raylib.h"
#include <vector>

#include "Painter.h"
#include "PointCloud.h"




const mode selected_mode = mode::random_points;
const int numberPoints = 20;

const int winWidth = 800;
const int winHeight = 600;

const int drawBorder = 20;



int main() {
    // create points
    PointCloud points;
    Painter painter;

    points.CreatePointCloud(winWidth, winHeight, drawBorder, numberPoints, selected_mode);

    InitWindow(winWidth, winHeight, "ADALGO - TASK1");
    SetTargetFPS(60);


    //DrawCall
    while (!WindowShouldClose()) {
        BeginDrawing();


        for (int i = 0; i < numberPoints; i++) {






            painter.PaintPoint(points.GetPointCloud()[i]);

            painter.PaintLine(points.GetMinXPoint(), points.GetMaxYPoint());
            painter.PaintLine(points.GetMaxYPoint(), points.GetMaxXPoint());
            painter.PaintLine(points.GetMaxXPoint(), points.GetMinYPoint());
            painter.PaintLine(points.GetMinYPoint(), points.GetMinXPoint());

        }
        ClearBackground(RAYWHITE);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
