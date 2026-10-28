#include "raylib.h"

int main()
{
    // Initialize the game window
    const int screenWidth = 960;
    const int screenHeight = 540;

    InitWindow(screenWidth, screenHeight, "RetroBIRD Engine");
    SetTargetFPS(60);

    // Configure the 3D camera
    Camera3D camera = {};
    camera.position = { 8.0f, 8.0f, 12.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Create our temporary bird object
    Vector3 birdPosition = { 0.0f, 0.5f, 0.0f };

    // Main game loop
    while (!WindowShouldClose())
    {
        // Calculate movement using frame time
        float speed = 5.0f * GetFrameTime();

		// Handle keyboard movement
		if (IsKeyDown(KEY_W)) birdPosition.z -= speed;
		if (IsKeyDown(KEY_S)) birdPosition.z += speed;
		if (IsKeyDown(KEY_A)) birdPosition.x -= speed;
		if (IsKeyDown(KEY_D)) birdPosition.x += speed;

		// Handle vertical movement
		if (IsKeyDown(KEY_SPACE)) birdPosition.y += speed;
		if (IsKeyDown(KEY_LEFT_CONTROL)) birdPosition.y -= speed;

		// Prevent the bird from going below the ground
		if (birdPosition.y < 0.5f)
		{
			birdPosition.y = 0.5f;
		}

		// Update the camera to follow the bird
		camera.target = birdPosition;

		camera.position = {
			birdPosition.x + 8.0f,
			birdPosition.y + 8.0f,
			birdPosition.z + 12.0f
		};

        // Begin rendering
        BeginDrawing();
        ClearBackground(SKYBLUE);

        // Render the 3D world
        BeginMode3D(camera);

        DrawGrid(20, 1.0f);
        DrawCube(birdPosition, 1.0f, 1.0f, 1.0f, RED);
        DrawCubeWires(birdPosition, 1.0f, 1.0f, 1.0f, BLACK);

        EndMode3D();

        // Render the 2D interface
        DrawText("RetroBIRD Engine", 20, 20, 20, BLACK);
		DrawText("WASD: Move", 20, 50, 18, DARKGRAY);
		DrawText("SPACE: Ascend", 20, 75, 18, DARKGRAY);
		DrawText("LEFT CTRL: Descend", 20, 100, 18, DARKGRAY);
        DrawFPS(10, 80);

        EndDrawing();
    }

    // Shut down the application
    CloseWindow();

    return 0;
}
