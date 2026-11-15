#include <iostream>
#include "raylib.h"
#include <vector>
#include <string>

#include <chrono>

#include "GiftWrapping.h"
#include "QuickHull.h"
#include "Painter.h"
#include "PointCloud.h"
#include "ConvexHull.h"

// HIER KONFIGURIEREN
//---------------------------------------

// in welcher Form werden die Punkte erschaffen
const pointcloud_mode selectedMode = pointcloud_mode::circle;

// Performance oder Visual
const program_mode programMode = program_mode::visual;

// Welcher Algorithmus für visual
const algorithm algorithm = algorithm::quickqull;

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
    QuickHull qh;

    //float progress = 0.0f;
    //const float secondsPerLine = 0.5f;


    if (selectedMode == pointcloud_mode::file) {
        points.CreatePointCloudFile(std::string(PROJECT_DIR) + "/" + importFile);
    } else {
        points.CreatePointCloud(winWidth, winHeight, drawBorder, numberPoints, selectedMode);
    }

    if (points.GetCountPoints() < 2) {
        std::cerr << "Mindestens 2 Punkte noetig, vorhanden: " << points.GetCountPoints() << std::endl;
        return 1;
    }

    gw.Init(points);

    if (programMode == program_mode::visual) {
        InitWindow(winWidth, winHeight, "ADALGO - TASK1");
        SetTargetFPS(60);

        // QuickHull vorbereiten (GiftWrapping wurde oben schon mit gw.Init initialisiert)
        qh.Init(points);


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

            // PAINT QUICKHULL
            if (algorithm == algorithm::quickqull) {
                if (IsKeyPressed(KEY_RIGHT)) {
                    qh.Step();
                }
                painter.PaintQuickHull(qh, points, winHeight);
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

        // Ergebnis: Startpunkt steht am Ende nochmal in der Hülle (zum Schließen) -> - 1
        std::cout << "Gift Wrapping Huellpunkte: " << gw.GetHull().GetCountPoints() - 1 << std::endl;

        // QUICKHULL: eigene Zeitmessung, damit sich die Zeiten nicht vermischen.
        // Init gehört hier mit in die Messung: dort passiert schon die erste Aufteilung (O(n))
        auto startQh = std::chrono::steady_clock::now();

        qh.Init(points);
        qh.StartAlgorithmPerformance(points);

        auto endQh = std::chrono::steady_clock::now();

        double msQh = std::chrono::duration<double, std::milli>(endQh - startQh).count();
        std::cout << "QuickHull mit " << points.GetCountPoints() << " Punkten: "
                << msQh << " ms" << std::endl;
        std::cout << "QuickHull Huellpunkte: " << qh.GetHull().GetCountPoints() - 1 << std::endl;
    }
    return 0;
}
