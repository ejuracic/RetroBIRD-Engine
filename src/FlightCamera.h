
#pragma once

#include "raylib.h"
#include "Bird.h"

class FlightCamera
{
public:
    FlightCamera();

    void Update(const Bird& bird);
    const Camera3D& GetCamera() const;

private:
    Camera3D camera_ = {};
};
