
#include "FlightCamera.h"

#include <algorithm>

// Name: FlightCamera
// Description: Initializes the third-person camera
// Parameters: None
// Returns: None
FlightCamera::FlightCamera()
{
    camera_.up = { 0.0f, 1.0f, 0.0f };
    camera_.fovy = 60.0f;
    camera_.projection = CAMERA_PERSPECTIVE;
}

// Name: Update
// Description: Positions the camera behind the bird
// Parameters: bird
// Returns: None
void FlightCamera::Update(const Bird& bird)
{
    Vector3 position = bird.GetPosition();
    Vector3 forward = bird.GetForward();

    camera_.position = {
        position.x - forward.x * 9.0f,
        position.y + 3.0f - forward.y * 9.0f,
        position.z - forward.z * 9.0f
    };

    camera_.position.y = std::max(
        camera_.position.y, 1.5f
    );

    camera_.target = {
        position.x + forward.x * 3.0f,
        position.y + forward.y * 3.0f,
        position.z + forward.z * 3.0f
    };
}

// Name: GetCamera
// Description: Returns the camera for rendering
// Parameters: None
// Returns: Constant Camera3D reference
const Camera3D& FlightCamera::GetCamera() const
{
    return camera_;
}
