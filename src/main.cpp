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

        // Update game systems
        if (!timeTrial.IsFinished())
        {
            playerBird.Update(deltaTime, input);

            timeTrial.Update(
                deltaTime,
                previousPosition,
                playerBird.GetPosition()
            );
        }

        flightCamera.Update(playerBird);

        // Begin rendering
        BeginDrawing();
        ClearBackground(SKYBLUE);

        BeginMode3D(flightCamera.GetCamera());

        DrawGrid(150, 1.0f);
        playerBird.Draw();
        timeTrial.Draw();

        EndMode3D();

        // Draw HUD
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

        // Display personal best
        if (timeTrial.HasBestTime())
        {
            DrawText(
                TextFormat(
                    "Best (session): %.2f seconds",
                    timeTrial.GetBestTime()
                ),
                20, 145, 20, DARKBLUE
            );
        }
        else
        {
            DrawText(
                "Best (session): --",
                20, 145, 20, DARKBLUE
            );
        }

        DrawText(
            "Gold: 22s | Silver: 30s | Bronze: 45s",
            20, 175, 18, DARKGRAY
        );

        // Display race completion
        if (timeTrial.IsFinished())
        {
            const char* medalText = "No Medal";
            Color medalColor = DARKGRAY;

            switch (timeTrial.GetMedal())
            {
                case TrialMedal::Gold:
                    medalText = "GOLD MEDAL!";
                    medalColor = GOLD;
                    break;

                case TrialMedal::Silver:
                    medalText = "SILVER MEDAL!";
                    medalColor = GRAY;
                    break;

                case TrialMedal::Bronze:
                    medalText = "BRONZE MEDAL!";
                    medalColor = BROWN;
                    break;

                case TrialMedal::None:
                    break;
            }

            DrawRectangle(
                235, 235, 490, 160,
                Fade(RAYWHITE, 0.90f)
            );

            DrawText(
                "TIME TRIAL COMPLETE!",
                275, 250, 30, DARKGREEN
            );

            DrawText(
                medalText,
                345, 300, 28, medalColor
            );

            DrawText(
                "Press R to race again",
                340, 350, 20, BLACK
            );
        }

        DrawFPS(10, 210);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
