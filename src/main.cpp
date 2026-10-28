
#include "raylib.h"

#include "Bird.h"
#include "FlightCamera.h"

int main()
{
    // Initialize the window
    InitWindow(960, 540, "RetroBIRD Engine");
    SetTargetFPS(60);

    // Create the player bird and camera
    Bird playerBird;
    FlightCamera flightCamera;

    flightCamera.Update(playerBird);

    // Main game loop
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        // Read player input
        FlightInput input = {};

        if (IsKeyDown(KEY_A)) input.turn -= 1.0f;
        if (IsKeyDown(KEY_D)) input.turn += 1.0f;

        if (IsKeyDown(KEY_W)) input.pitch += 1.0f;
        if (IsKeyDown(KEY_S)) input.pitch -= 1.0f;

        // Update game systems
        playerBird.Update(deltaTime, input);
        flightCamera.Update(playerBird);

        // Begin rendering
        BeginDrawing();
        ClearBackground(SKYBLUE);

        BeginMode3D(flightCamera.GetCamera());

        DrawGrid(100, 1.0f);
        playerBird.Draw();

        EndMode3D();

        // Draw interface
        DrawText("RetroBIRD Engine", 20, 20, 20, BLACK);
        DrawText("W/S: Pitch", 20, 50, 18, DARKGRAY);
        DrawText("A/D: Turn", 20, 75, 18, DARKGRAY);
        DrawFPS(10, 105);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
