#include "raylib.h"

#include "Bird.h"
#include "FlightCamera.h"
#include "Checkpoint.h"

int main()
{
    // Initialize the game window
    InitWindow(960, 540, "RetroBIRD Engine");
    SetTargetFPS(60);

    // Initialize game objects
    Bird playerBird;
    FlightCamera flightCamera;

    Checkpoint checkpoint(
        { 0.0f, 5.0f, -20.0f },
        3.0f
    );

    flightCamera.Update(playerBird);

    Vector3 previousPosition = playerBird.GetPosition();
    bool checkpointPassed = false;

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

        // Remember position before movement
        previousPosition = playerBird.GetPosition();

        // Update the player and camera
        playerBird.Update(deltaTime, input);
        flightCamera.Update(playerBird);

        // Detect checkpoint crossing
        if (!checkpointPassed &&
            checkpoint.CheckCrossing(
                previousPosition,
                playerBird.GetPosition()))
        {
            checkpointPassed = true;
        }

        // Begin rendering
        BeginDrawing();
        ClearBackground(SKYBLUE);

        BeginMode3D(flightCamera.GetCamera());

        DrawGrid(100, 1.0f);
        playerBird.Draw();

        if (!checkpointPassed)
        {
            checkpoint.Draw();
        }

        EndMode3D();

        // Draw the interface
        DrawText("RetroBIRD Engine", 20, 20, 20, BLACK);
        DrawText("W/S: Pitch", 20, 50, 18, DARKGRAY);
        DrawText("A/D: Turn", 20, 75, 18, DARKGRAY);

        if (checkpointPassed)
        {
            DrawText(
                "CHECKPOINT COMPLETE!",
                300, 50, 28, DARKGREEN
            );
        }
        else
        {
            DrawText(
                "Fly through the golden ring!",
                300, 50, 20, MAROON
            );
        }

        DrawFPS(10, 105);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
