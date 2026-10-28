#include "raylib.h"

#include "Bird.h"
#include "FlightCamera.h"
#include "TimeTrial.h"

int main()
{
    // Initialize the game window
    InitWindow(960, 540, "RetroBIRD Engine");
    SetTargetFPS(60);

    // Initialize game objects
    Bird playerBird;
    FlightCamera flightCamera;
    TimeTrial timeTrial;

    flightCamera.Update(playerBird);

    // Main game loop
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        // Restart the time trial
        if (IsKeyPressed(KEY_R))
        {
            playerBird = Bird{};
            timeTrial.Reset();
            flightCamera.Update(playerBird);
        }

        // Read player input
        FlightInput input = {};

        if (IsKeyDown(KEY_A)) input.turn -= 1.0f;
        if (IsKeyDown(KEY_D)) input.turn += 1.0f;

        if (IsKeyDown(KEY_W)) input.pitch += 1.0f;
        if (IsKeyDown(KEY_S)) input.pitch -= 1.0f;

        // Record position before movement
        Vector3 previousPosition = playerBird.GetPosition();

        // Update game systems while racing
        if (!timeTrial.IsFinished())
        {
            playerBird.Update(deltaTime, input);

            timeTrial.Update(
                deltaTime,
                previousPosition,
                playerBird.GetPosition()
            );
        }

        // Update camera
        flightCamera.Update(playerBird);

        // Begin rendering
        BeginDrawing();
        ClearBackground(SKYBLUE);

        BeginMode3D(flightCamera.GetCamera());

        // Render the environment
        DrawGrid(150, 1.0f);

        // Render the bird
        playerBird.Draw();

        // Render the checkpoint course
        timeTrial.Draw();

        EndMode3D();

        // Draw game interface
        DrawText(
            "RetroBIRD Engine",
            20, 20, 20, BLACK
        );

        DrawText(
            "W/S: Pitch | A/D: Turn | R: Restart",
            20, 50, 18, DARKGRAY
        );

        DrawText(
            TextFormat(
                "Time: %.2f seconds",
                timeTrial.GetElapsedTime()
            ),
            20, 85, 22, BLACK
        );

        DrawText(
            TextFormat(
                "Checkpoints: %d / %d",
                static_cast<int>(timeTrial.GetPassedCount()),
                static_cast<int>(timeTrial.GetTotalCount())
            ),
            20, 115, 22, BLACK
        );

        // Display race completion
        if (timeTrial.IsFinished())
        {
            DrawText(
                "TIME TRIAL COMPLETE!",
                280, 190, 30, DARKGREEN
            );

            DrawText(
                "Press R to race again",
                330, 240, 20, BLACK
            );
        }

        DrawFPS(10, 155);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
