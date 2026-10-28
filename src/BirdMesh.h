#pragma once

#include "raylib.h"

// Represents the visual geometry of a bird
class BirdMesh
{
public:
    // Name: Draw
    // Description: Draws the procedural bird geometry
    // Parameters: wingAngle in degrees
    // Returns: None
    void Draw(float wingAngle) const;
};
