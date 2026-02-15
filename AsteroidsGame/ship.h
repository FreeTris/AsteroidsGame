#pragma once
#include "raylib.h"

struct Ship
{
    Vector2 position;
    Vector2 velocity;
    float rotation; // degrees

    void FixedUpdate(float dt); // fixed timestep seconds
    void Draw() const;
};
