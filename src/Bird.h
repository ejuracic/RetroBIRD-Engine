
#pragma once

#include "raylib.h"
#include "BirdMesh.h"

// Represents player or AI flight commands
struct FlightInput
{
    float turn = 0.0f;
    float pitch = 0.0f;
};

class Bird
{
public:
    void Update(float deltaTime, const FlightInput& input);
    void Draw() const;

    Vector3 GetPosition() const;
    Vector3 GetForward() const;

private:
    // Flight state
    Vector3 position_ = { 0.0f, 5.0f, 0.0f };
    Vector3 forward_ = { 0.0f, 0.0f, -1.0f };

    float yaw_ = 0.0f;
    float pitch_ = 0.0f;
    float bankAngle_ = 0.0f;

    // Animation state
    float wingPhase_ = 0.0f;

    // Procedural bird representation
    BirdMesh mesh_;
};
