#include "raylib.h"

int main()  {
    InitWindow(600,400,"New game");

    while(!WindowShouldClose())  {

        BeginDrawing();

        ClearBackground(SKYBLUE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}