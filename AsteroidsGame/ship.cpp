#include "ship.h"
#include "raymath.h"
#include <cmath>

void Ship::FixedUpdate(float dt)
{
    const float turnSpeed = 180.0f;  // degrees/sec
    const float thrustAccel = 200.0f;  // units/sec^2

    // Rotate
    if (IsKeyDown(KEY_LEFT))  rotation -= turnSpeed * dt;
    if (IsKeyDown(KEY_RIGHT)) rotation += turnSpeed * dt;

    // Thrust
    if (IsKeyDown(KEY_UP))
    {
        float r = rotation * DEG2RAD;
        Vector2 dir = { cosf(r), sinf(r) };
        Vector2 accel = Vector2Scale(dir, thrustAccel);
        velocity = Vector2Add(velocity, Vector2Scale(accel, dt));
    }

    // Optional: simple damping (uncomment if you want less drifting)
    // velocity = Vector2Scale(velocity, 0.99f);

    // Integrate (Euler)
    position = Vector2Add(position, Vector2Scale(velocity, dt));

    // Wrap using current window size
    float w = (float)GetScreenWidth();
    float h = (float)GetScreenHeight();

    if (position.x > w) position.x = 0;
    if (position.x < 0) position.x = w;
    if (position.y > h) position.y = 0;
    if (position.y < 0) position.y = h;
}

void Ship::Draw() const
{
    float r = rotation * DEG2RAD;

    // Ship triangle in local space (points +X)
    Vector2 p1 = { 16,  0 };   // nose
    Vector2 p2 = { -10, -8 };  // back top
    Vector2 p3 = { -10,  8 };  // back bottom

    Vector2 w1 = Vector2Add(position, Vector2Rotate(p1, r));
    Vector2 w2 = Vector2Add(position, Vector2Rotate(p2, r));
    Vector2 w3 = Vector2Add(position, Vector2Rotate(p3, r));

    DrawTriangleLines(w1, w2, w3, WHITE);

    // --- Thrust flame ---
    if (IsKeyDown(KEY_UP))
    {
        // Flame triangle behind the ship (also in local space)
        Vector2 f1 = { -16,  0 };  // flame tip (further back)
        Vector2 f2 = { -10, -5 };  // flame top
        Vector2 f3 = { -10,  5 };  // flame bottom

        Vector2 fw1 = Vector2Add(position, Vector2Rotate(f1, r));
        Vector2 fw2 = Vector2Add(position, Vector2Rotate(f2, r));
        Vector2 fw3 = Vector2Add(position, Vector2Rotate(f3, r));

        DrawTriangleLines(fw1, fw2, fw3, ORANGE);
    }
}
