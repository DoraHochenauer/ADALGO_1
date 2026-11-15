#include <iostream>
#include "raylib.h"
#include <vector>
#include <string>

#include <chrono>

#include "GiftWrapping.h"
#include "Painter.h"
#include "PointCloud.h"
#include "ConvexHull.h"

// HIER KONFIGURIEREN
//---------------------------------------

// in welcher Form werden die Punkte erschaffen
const pointcloud_mode selectedMode = pointcloud_mode::line;

// Performance oder Visual
const program_mode programMode = program_mode::visual;

// Welcher Algorithmus für visual
const algorithm algorithm = algorithm::gift_wrapping;

//wieviele Punkte (nicht bei pointcloud_mode::file, dort steht die Anzahl in der Datei)
const int numberPoints = 5;

// Datei für pointcloud_mode::file (relativ zum Projektordner)
const std::string importFile = "importPoints";

//---------------------------------------


const int winWidth = 800;
const int winHeight = 600;

const int drawBorder = 20;


int main() {
    //MIND 2 PUNKTE!


    PointCloud points;
    Painter painter;

    GiftWrapping gw;

    if (selectedMode == pointcloud_mode::file) {
        points.CreatePointCloudFile(std::string(PROJECT_DIR) + "/" + importFile);
    } else {
        points.CreatePointCloud(winWidth, winHeight, drawBorder, numberPoints, selectedMode);
    }

    if (points.GetCountPoints() < 2) {
        std::cerr << "Mindestens 2 Punkte angeben! " << points.GetCountPoints() << std::endl;
        return 1;
    }

    gw.Init(points);

    if (programMode == program_mode::visual) {
        InitWindow(winWidth, winHeight, "ADALGO - TASK1");
        SetTargetFPS(60);


        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(RAYWHITE);

            // Quickqull hier!

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
        //Quickhull hier!

        auto end = std::chrono::steady_clock::now();

        double ms = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "Gift Wrapping mit " << points.GetCountPoints() << " Punkten: "
                << ms << " ms" << std::endl;
    }
    return 0;
}
