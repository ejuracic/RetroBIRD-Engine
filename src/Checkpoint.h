#pragma once

#include "raylib.h"

class Checkpoint
{
public:
    Checkpoint(Vector3 position, float radius);

    void Draw() const;
    bool CheckCrossing(Vector3 previousPosition,
                       Vector3 currentPosition) const;

    Vector3 GetPosition() const;

private:
    Vector3 position_;
    float radius_;
};
