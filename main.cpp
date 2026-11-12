#include <iostream>
#include "raylib.h"
#include <vector>

#include "PointCloud.h"




const mode selected_mode = mode::random_points;
const int numberPoints = 5;

const int winWidth = 800;
const int winHeight = 600;

const int drawBorder = 20;



int main() {
    // create points
    PointCloud* points = new PointCloud();

    points->CreatePointCloud(winWidth, winHeight, drawBorder, numberPoints, selected_mode);

    InitWindow(winWidth, winHeight, "ADALGO - TASK1");
    SetTargetFPS(60);


    //DrawCall
    while (!WindowShouldClose()) {
        BeginDrawing();


        for (int i = 0; i < numberPoints; i++) {
            DrawCircle(points->GetPointCloud()[i].x, points->GetPointCloud()[i].y, 4, BLUE);
        }
        ClearBackground(RAYWHITE);
        EndDrawing();
    }
    CloseWindow();

    delete points;
    return 0;
}
