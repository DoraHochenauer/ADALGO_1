#include <iostream>
#include "raylib.h"
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

#include <chrono>

#include "GiftWrapping.h"
#include "QuickHull.h"
#include "Painter.h"
#include "PointCloud.h"
#include "ConvexHull.h"

// HIER KONFIGURIEREN
//---------------------------------------

// in welcher Form werden die Punkte erschaffen
const pointcloud_mode pointcloud_mode = pointcloud_mode::file;

// Performance oder Visual
const program_mode programMode = program_mode::visual;

// Welcher Algorithmus für visual
const algorithm algorithm = algorithm::quickqull;

//wieviele Punkte (nicht bei pointcloud_mode::file, dort steht die Anzahl in der Datei)
const int numberPoints = 15;

// Datei für pointcloud_mode::file (relativ zum Projektordner)
const std::string importFile = "importPoints10000";

//---------------------------------------


const int winWidth = 800;
const int winHeight = 600;

const int drawBorder = 20;


// Skaliert und verschiebt alle Punkte so, dass sie ins Fenster passen für visual mode
void MapToWindow(PointCloud &points) {
    float minX = points.GetMinX(), maxX = points.GetMaxX();
    float minY = points.GetMinY(), maxY = points.GetMaxY();

    float rangeX = maxX - minX;
    float rangeY = maxY - minY;
    float availableW = winWidth - 2.0f * drawBorder;
    float availableH = winHeight - 2.0f * drawBorder;
    float scaleX = rangeX > 0 ? availableW / rangeX : INFINITY;
    float scaleY = rangeY > 0 ? availableH / rangeY : INFINITY;
    float scale = std::min(scaleX, scaleY);
    if (scale == INFINITY) scale = 1.0f;   // alle Punkte identisch

    // Mitte der Punktwolke kommt in die Fenstermitte
    float centerX = (minX + maxX) / 2.0f;
    float centerY = (minY + maxY) / 2.0f;

    PointCloud mapped;
    for (const Point &p: points.GetPointCloud()) {
        Vector2 v = p.GetPoint();
        mapped.PushPoint(Point(Vector2{
                                   winWidth / 2.0f + (v.x - centerX) * scale,
                                   winHeight / 2.0f + (v.y - centerY) * scale
                               }, p.GetColor()));   // Farbe (rote Extrempunkte) mitnehmen
    }
    points = mapped;
}


int main() {

    PointCloud points;
    Painter painter;

    GiftWrapping gw;
    QuickHull qh;

    if (pointcloud_mode == pointcloud_mode::file) {
        points.CreatePointCloudFile(std::string(PROJECT_DIR) + "/" + importFile);
    } else {
        points.CreatePointCloud(winWidth, winHeight, drawBorder, numberPoints, pointcloud_mode);
    }
    //MIND 2 PUNKTE!
    if (points.GetCountPoints() < 2) {
        std::cerr << "Mindestens 2 Punkte angeben! " << points.GetCountPoints() << std::endl;
        return 1;
    }

    if (pointcloud_mode == pointcloud_mode::file && programMode == program_mode::visual) {
        MapToWindow(points);
    }

    gw.Init(points);
    // QuickHull vorbereiten (GiftWrapping wurde oben schon mit gw.Init initialisiert)
    qh.Init(points);

    if (programMode == program_mode::visual) {
        InitWindow(winWidth, winHeight, "ADALGO - TASK1");
        SetTargetFPS(60);



        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(RAYWHITE);

            //PAINT GIFTWRAPPING
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
