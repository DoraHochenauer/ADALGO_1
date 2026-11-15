#include <iostream>
#include "raylib.h"
#include <vector>

#include "GiftWrapping.h"
#include "Painter.h"
#include "PointCloud.h"
#include "raymath.h"


const pointcloud_mode selectedMode = pointcloud_mode::random_points;

const program_mode programMode = program_mode::visual;
const algorithm algorithm = algorithm::gift_wrapping;
const int numberPoints = 5;

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
        gw.Init(points);

        while (!WindowShouldClose()) {
            progress += GetFrameTime() / secondsPerLine;


            if (IsKeyPressed(KEY_RIGHT)) {
                gw.Step();
            }

            BeginDrawing();
            ClearBackground(RAYWHITE);

            // GIFTWRAPPING
            if (algorithm == algorithm::gift_wrapping) {
                DrawText("Pfeil rechts = weiter", 10, 10, 20, DARKGRAY);
                Point &currentPoint = points.GetPoint(gw.GetCurrent());
                if (gw.GetState() == giftwrapp_state::find_best_favourite) {
                    if (gw.GetFavourtiteCandidate() < points.GetCountPoints()) {
                        painter.PaintDashedLine(currentPoint, points.GetPoint(gw.GetFavourtiteCandidate()), LIGHTGRAY);
                    }
                }
                if (gw.GetState() == giftwrapp_state::find_best_favourite ||
                    gw.GetState() == giftwrapp_state::add_favourite) {
                    // orange: aktueller Favorit
                    painter.PaintDashedLine(currentPoint, points.GetPoint(gw.GetFavourite()), ORANGE);
                    }

                for (Point &p: points.GetPointCloud()) {
                    painter.PaintPoint(p);
                }
                std::vector<Point> &hull = gw.GetHull().GetPointCloud();
                for (size_t i = 0; i + 1 < hull.size(); i++) {
                    painter.PaintLine(hull[i], hull[i + 1]);
                }

                DrawText(TextFormat("Zustand: %s", gw.GetStateName()), 10, winHeight - 30, 20, DARKGRAY);
            }
            EndDrawing();
        }

        CloseWindow();
    }
    return 0;
}
