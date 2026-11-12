#include <iostream>
#include "raylib.h"
#include <cstdlib>
#include <vector>
#include <ctime>



int randomInt(int min, int max) {
    return rand() % (max - min + 1) + min;
}

const int numberPoints = 5;

const int winWidth = 800;
const int winHeight = 600;

const int drawBorder = 20;

std::vector<Vector2> Pos;


int main() {
    srand(time(0));

    InitWindow(winWidth, winHeight, "ADALGO - TASK1");
    SetTargetFPS(60);

    for (int i = 0; i < numberPoints; i++) {
        Pos.push_back(Vector2{(float)randomInt(drawBorder, winWidth - drawBorder), (float)randomInt(drawBorder, winHeight - drawBorder)});

    }

    while (!WindowShouldClose()) {
        BeginDrawing();


        for (int i = 0; i < numberPoints; i++) {
            DrawCircle(Pos[i].x, Pos[i].y, 4, BLUE);
        }
        ClearBackground(RAYWHITE);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
