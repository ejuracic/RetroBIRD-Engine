#include "raylib.h"
#include <cmath>

int main()
{
    // Initialize the game window
    const int screenWidth = 960;
    const int screenHeight = 540;

    InitWindow(screenWidth, screenHeight, "RetroBIRD Engine");
    SetTargetFPS(60);

    // Initialize the 3D camera
    Camera3D camera = {};
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Initialize the bird's position
    Vector3 birdPosition = { 0.0f, 5.0f, 0.0f };

    // Initialize flight variables
    float yaw = 0.0f;
    float pitch = 0.0f;

    const float flightSpeed = 6.0f;
    const float turnSpeed = 1.5f;

    // Main game loop
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        // Update horizontal turning
        if (IsKeyDown(KEY_A))
        {
            yaw -= turnSpeed * deltaTime;
        }

        if (IsKeyDown(KEY_D))
        {
            yaw += turnSpeed * deltaTime;
        }

        // Update vertical turning
        if (IsKeyDown(KEY_W))
        {
            pitch += turnSpeed * deltaTime;
        }

        if (IsKeyDown(KEY_S))
        {
            pitch -= turnSpeed * deltaTime;
        }

        // Limit the vertical flight angle
        if (pitch > 0.75f) pitch = 0.75f;
        if (pitch < -0.75f) pitch = -0.75f;

        // Calculate forward direction
        Vector3 forward = {
            std::sin(yaw) * std::cos(pitch),
            std::sin(pitch),
            -std::cos(yaw) * std::cos(pitch)
        };

        // Move the bird forward automatically
        birdPosition.x += forward.x * flightSpeed * deltaTime;
        birdPosition.y += forward.y * flightSpeed * deltaTime;
        birdPosition.z += forward.z * flightSpeed * deltaTime;

        // Prevent the bird from entering the ground
        if (birdPosition.y < 0.5f)
        {
            birdPosition.y = 0.5f;
        }

        // Position the camera behind the bird
        camera.position = {
            birdPosition.x - forward.x * 9.0f,
            birdPosition.y + 3.0f - forward.y * 9.0f,
            birdPosition.z - forward.z * 9.0f
        };

        // Keep the camera above the ground
        if (camera.position.y < 1.5f)
        {
            camera.position.y = 1.5f;
        }

        // Look slightly ahead of the bird
        camera.target = {
            birdPosition.x + forward.x * 3.0f,
            birdPosition.y + forward.y * 3.0f,
            birdPosition.z + forward.z * 3.0f
        };

        // Begin rendering
        BeginDrawing();
        ClearBackground(SKYBLUE);

        BeginMode3D(camera);

        // Draw the environment
        DrawGrid(100, 1.0f);

        // Draw the temporary bird
        DrawCube(birdPosition, 1.0f, 1.0f, 1.0f, RED);
        DrawCubeWires(birdPosition, 1.0f, 1.0f, 1.0f, BLACK);

        // Draw a green indicator for forward direction
        Vector3 nosePosition = {
            birdPosition.x + forward.x * 2.0f,
            birdPosition.y + forward.y * 2.0f,
            birdPosition.z + forward.z * 2.0f
        };

        DrawLine3D(birdPosition, nosePosition, GREEN);
        DrawSphere(nosePosition, 0.12f, GREEN);

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
