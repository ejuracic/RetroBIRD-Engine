
#include "raylib.h"
#include "rlgl.h"
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
    float bankAngle = 0.0f;

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

        // Determine the desired banking angle
        float targetBank = 0.0f;

        if (IsKeyDown(KEY_A))
        {
            targetBank = 25.0f;
        }

        if (IsKeyDown(KEY_D))
        {
            targetBank = -25.0f;
        }

        // Smoothly transition toward the desired bank angle
        float bankBlend = 5.0f * deltaTime;

        if (bankBlend > 1.0f)
        {
            bankBlend = 1.0f;
        }

        bankAngle += (targetBank - bankAngle) * bankBlend;

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

        // Begin rendering the 3D world
        BeginMode3D(camera);

        // Draw the environment
        DrawGrid(100, 1.0f);

        // Save the current transformation
        rlPushMatrix();

        // Move the bird to its world position
        rlTranslatef(
            birdPosition.x,
            birdPosition.y,
            birdPosition.z
        );

        // Apply horizontal rotation
        rlRotatef(-yaw * RAD2DEG, 0.0f, 1.0f, 0.0f);

        // Apply vertical rotation
        rlRotatef(pitch * RAD2DEG, 1.0f, 0.0f, 0.0f);

        // Apply banking rotation
        rlRotatef(bankAngle, 0.0f, 0.0f, 1.0f);

        // Draw the temporary bird body
        DrawCube(
            { 0.0f, 0.0f, 0.0f },
            1.2f, 0.5f, 2.0f,
            RED
        );

        DrawCubeWires(
            { 0.0f, 0.0f, 0.0f },
            1.2f, 0.5f, 2.0f,
            BLACK
        );

        // Draw the forward-facing indicator
        DrawLine3D(
            { 0.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, -2.0f },
            GREEN
        );

        DrawSphere(
            { 0.0f, 0.0f, -2.0f },
            0.12f,
            GREEN
        );

        // Restore the previous transformation
        rlPopMatrix();

        // End rendering the 3D world
        EndMode3D();

        // Draw the interface
        DrawText("RetroBIRD Engine", 20, 20, 20, BLACK);
        DrawText("W/S: Pitch", 20, 50, 18, DARKGRAY);
        DrawText("A/D: Turn", 20, 75, 18, DARKGRAY);
        DrawFPS(10, 105);

        EndDrawing();
    }

    // Shut down the application
    CloseWindow();

    return 0;
}
