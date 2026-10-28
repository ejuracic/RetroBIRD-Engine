#include "Checkpoint.h"

#include <cmath>

namespace
{
    constexpr int kSegments = 32;
    constexpr float kRingThickness = 0.15f;
}

// Name: Checkpoint
// Description: Initializes a racing checkpoint
// Parameters: position, radius
// Returns: None
Checkpoint::Checkpoint(Vector3 position, float radius)
    : position_(position), radius_(radius)
{
}

// Name: Draw
// Description: Draws a vertical circular checkpoint ring
// Parameters: None
// Returns: None
void Checkpoint::Draw() const
{
    for (int i = 0; i < kSegments; i++)
    {
        float angle1 = 2.0f * PI * i / kSegments;
        float angle2 = 2.0f * PI * (i + 1) / kSegments;

        Vector3 start = {
            position_.x + std::cos(angle1) * radius_,
            position_.y + std::sin(angle1) * radius_,
            position_.z
        };

        Vector3 end = {
            position_.x + std::cos(angle2) * radius_,
            position_.y + std::sin(angle2) * radius_,
            position_.z
        };

        DrawCylinderEx(
            start,
            end,
            kRingThickness,
            kRingThickness,
            6,
            GOLD
        );
    }
}

// Name: CheckCrossing
// Description: Checks whether the bird crosses the ring plane
// Parameters: Previous and current bird positions
// Returns: True if the bird passes through the ring
bool Checkpoint::CheckCrossing(
    Vector3 previousPosition,
    Vector3 currentPosition) const
{
    float previousZ = previousPosition.z - position_.z;
    float currentZ = currentPosition.z - position_.z;

    // The bird must cross the ring from front to back
    if (previousZ <= 0.0f || currentZ > 0.0f)
    {
        return false;
    }

    float denominator = previousZ - currentZ;

    if (denominator <= 0.0f)
    {
        return false;
    }

    // Find the precise point where the flight path crosses
    float t = previousZ / denominator;

    float crossingX = previousPosition.x +
        (currentPosition.x - previousPosition.x) * t;

    float crossingY = previousPosition.y +
        (currentPosition.y - previousPosition.y) * t;

    float dx = crossingX - position_.x;
    float dy = crossingY - position_.y;

    // Verify the crossing is inside the ring
    return dx * dx + dy * dy < radius_ * radius_;
}

// Name: GetPosition
// Description: Returns checkpoint position
// Parameters: None
// Returns: Vector3 position
Vector3 Checkpoint::GetPosition() const
{
    return position_;
}
