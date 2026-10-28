
#include "Bird.h"
#include "rlgl.h"

#include <algorithm>
#include <cmath>

namespace
{
    constexpr float kFlightSpeed = 6.0f;
    constexpr float kTurnSpeed = 1.5f;
    constexpr float kMaxPitch = 0.75f;
    constexpr float kMaxBankAngle = 25.0f;
    constexpr float kBankSmoothing = 5.0f;
}

// Name: Update
// Description: Updates bird movement and flight orientation
// Parameters: deltaTime, input
// Returns: None
void Bird::Update(float deltaTime, const FlightInput& input)
{
    // Update flight angles
    yaw_ += input.turn * kTurnSpeed * deltaTime;

    pitch_ += input.pitch * kTurnSpeed * deltaTime;
    pitch_ = std::clamp(pitch_, -kMaxPitch, kMaxPitch);

    // Calculate visual banking
    float targetBank = -input.turn * kMaxBankAngle;

    float blend = std::clamp(
        kBankSmoothing * deltaTime, 0.0f, 1.0f
    );

    bankAngle_ += (targetBank - bankAngle_) * blend;

    // Calculate forward direction
    forward_ = {
        std::sin(yaw_) * std::cos(pitch_),
        std::sin(pitch_),
        -std::cos(yaw_) * std::cos(pitch_)
    };

    // Move forward
    position_.x += forward_.x * kFlightSpeed * deltaTime;
    position_.y += forward_.y * kFlightSpeed * deltaTime;
    position_.z += forward_.z * kFlightSpeed * deltaTime;

    // Prevent movement below ground
    position_.y = std::max(position_.y, 0.5f);
}

// Name: Draw
// Description: Renders the bird's temporary 3D representation
// Parameters: None
// Returns: None
void Bird::Draw() const
{
    rlPushMatrix();

    rlTranslatef(position_.x, position_.y, position_.z);

    rlRotatef(-yaw_ * RAD2DEG, 0.0f, 1.0f, 0.0f);
    rlRotatef(pitch_ * RAD2DEG, 1.0f, 0.0f, 0.0f);
    rlRotatef(bankAngle_, 0.0f, 0.0f, 1.0f);

    // Temporary bird body
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

    // Forward direction indicator
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

    rlPopMatrix();
}

// Name: GetPosition
// Description: Returns the bird's position
// Parameters: None
// Returns: Vector3 position
Vector3 Bird::GetPosition() const
{
    return position_;
}

// Name: GetForward
// Description: Returns the bird's forward direction
// Parameters: None
// Returns: Vector3 direction
Vector3 Bird::GetForward() const
{
    return forward_;
}
