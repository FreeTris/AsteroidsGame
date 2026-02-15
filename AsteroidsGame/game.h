#pragma once
#include "raylib.h"
#include "ship.h"
#include <vector>

struct Bullet {
    Vector2 position{};
    Vector2 velocity{};
    float life = 0.0f;
};

struct Asteroid {
    Vector2 position{};
    Vector2 velocity{};
    float radius = 0.0f;
    int size = 3;
};

enum class Mode { Playing, GameOver };

class Game {
public:
    void Init(float w, float h);
    void Restart(float w, float h);
    void UpdateFrame(float w, float h);
    void Render(float w, float h) const;

private:
    void FixedStep(float dt, float w, float h);

    // ✅ ADD THESE (your .cpp uses them)
    void UpdateEntities(float dt, float w, float h);
    void HandleCollisions(float w, float h);
    void HandleNextWave(float w, float h);

    void ResetShip(float w, float h);
    void SpawnWave(int count, float w, float h);

    static void Wrap(Vector2& p, float w, float h);
    static float WrapDelta(float d, float size);

private:
    Mode mode = Mode::Playing;
    Ship ship{};

    std::vector<Bullet> bullets;
    std::vector<Asteroid> asteroids;

    int score = 0;
    int wave = 1;
    int lives = 3;

    bool prevSpaceDown = false;

    float invulnTimer = 0.0f;
    const float invulnDuration = 1.5f;

    const double fixedDelta = 1.0 / 60.0;
    double accumulator = 0.0;
    double previousTime = 0.0;
};
