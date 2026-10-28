
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

    constexpr float kWingFlapSpeed = 7.0f;
    constexpr float kWingFlapAmplitude = 25.0f;
}

// Name: Update
// Description: Updates bird movement and animation
// Parameters: deltaTime, input
// Returns: None
void Bird::Update(float deltaTime, const FlightInput& input)
{
    // Update horizontal and vertical rotation
    yaw_ += input.turn * kTurnSpeed * deltaTime;

    pitch_ += input.pitch * kTurnSpeed * deltaTime;
    pitch_ = std::clamp(pitch_, -kMaxPitch, kMaxPitch);

    // Calculate banking
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

    // Update position
    position_.x += forward_.x * kFlightSpeed * deltaTime;
    position_.y += forward_.y * kFlightSpeed * deltaTime;
    position_.z += forward_.z * kFlightSpeed * deltaTime;

    // Prevent flight below the ground
    position_.y = std::max(position_.y, 0.5f);

    // Update wing animation
    wingPhase_ += kWingFlapSpeed * deltaTime;
}

// Name: Draw
// Description: Draws the oriented procedural bird
// Parameters: None
// Returns: None
void Bird::Draw() const
{
    // Calculate current wing position
    float wingAngle =
        std::sin(wingPhase_) * kWingFlapAmplitude;

    rlPushMatrix();

    // Apply world position
    rlTranslatef(position_.x, position_.y, position_.z);

    // Apply orientation
    rlRotatef(-yaw_ * RAD2DEG, 0.0f, 1.0f, 0.0f);
    rlRotatef(pitch_ * RAD2DEG, 1.0f, 0.0f, 0.0f);
    rlRotatef(bankAngle_, 0.0f, 0.0f, 1.0f);

    // Render procedural geometry
    mesh_.Draw(wingAngle);

    rlPopMatrix();
}

// Name: GetPosition
// Description: Returns bird position
// Parameters: None
// Returns: Vector3 position
Vector3 Bird::GetPosition() const
{
    return position_;
}

// Name: GetForward
// Description: Returns forward flight direction
// Parameters: None
// Returns: Vector3 direction
Vector3 Bird::GetForward() const
{
    return forward_;
}
