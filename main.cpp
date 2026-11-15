#include <iostream>
#include "raylib.h"
#include <vector>

#include <chrono>

#include "GiftWrapping.h"
#include "Painter.h"
#include "PointCloud.h"
#include "ConvexHull.h"


const pointcloud_mode selectedMode = pointcloud_mode::random_points;

const program_mode programMode = program_mode::visual;
const algorithm algorithm = algorithm::gift_wrapping;
const int numberPoints = 30;

const int winWidth = 800;
const int winHeight = 600;

const int drawBorder = 20;


int main() {
    PointCloud points;
    Painter painter;

    GiftWrapping gw;

    //float progress = 0.0f;
    //const float secondsPerLine = 0.5f;


    points.CreatePointCloud(winWidth, winHeight, drawBorder, numberPoints, selectedMode);
    gw.Init(points);

    if (programMode == program_mode::visual) {
        InitWindow(winWidth, winHeight, "ADALGO - TASK1");
        SetTargetFPS(60);


        while (!WindowShouldClose()) {
            // progress += GetFrameTime() / secondsPerLine;


            BeginDrawing();
            ClearBackground(RAYWHITE);

            // PAINT GIFTWRAPPING
            if (algorithm == algorithm::gift_wrapping) {
                if (IsKeyPressed(KEY_RIGHT)) {
                    gw.Step();
                }
                painter.PaintGiftWrapper(gw, points, winHeight);
            }
            EndDrawing();
        }

        CloseWindow();
    }


    if (programMode == program_mode::performance) {
        auto start = std::chrono::steady_clock::now();

            gw.StartAlgorithmPerformance(points);

        auto end = std::chrono::steady_clock::now();

        double ms = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "Gift Wrapping mit " << points.GetCountPoints() << " Punkten: "
                << ms << " ms" << std::endl;
    }
    return 0;
}
