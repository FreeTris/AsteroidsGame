#include "game.h"
#include <algorithm>
#include <cmath>

void Game::Init(float w, float h) {
    previousTime = GetTime();
    Restart(w, h);
}

void Game::Restart(float w, float h) {
    mode = Mode::Playing;
    score = 0;
    wave = 1;
    lives = 3;

    bullets.clear();
    asteroids.clear();

    SpawnWave(5, w, h);
    ResetShip(w, h);

    accumulator = 0.0;
    previousTime = GetTime();
}

void Game::UpdateFrame(float w, float h) {
    // restart works even if fixed step stalls
    if (mode == Mode::GameOver && IsKeyPressed(KEY_R)) {
        Restart(w, h);
        return;
    }

    // accumulator timing
    double currentTime = GetTime();
    double frameTime = currentTime - previousTime;
    previousTime = currentTime;

    if (frameTime > 0.25) frameTime = 0.25;
    accumulator += frameTime;

    int steps = 0;
    const int maxSteps = 5;

    while (accumulator >= fixedDelta && steps < maxSteps) {
        FixedStep((float)fixedDelta, w, h);
        accumulator -= fixedDelta;
        steps++;
    }

    if (steps == maxSteps) accumulator = 0.0;
}

void Game::FixedStep(float dt, float w, float h) {
    if (mode != Mode::Playing) return;

    // invulnerability
    if (invulnTimer > 0.0f) {
        invulnTimer -= dt;
        if (invulnTimer < 0.0f) invulnTimer = 0.0f;
    }

    // ship movement/controls
    ship.FixedUpdate(dt);

    // shoot (edge)
    bool spaceDown = IsKeyDown(KEY_SPACE);
    bool fireRequested = spaceDown && !prevSpaceDown;
    prevSpaceDown = spaceDown;

    if (fireRequested) {
        float r = ship.rotation * DEG2RAD;
        Vector2 dir = { cosf(r), sinf(r) };

        Bullet b{};
        b.position = ship.position;
        b.velocity = { dir.x * 500.0f + ship.velocity.x,
                       dir.y * 500.0f + ship.velocity.y };
        b.life = 1.0f;
        bullets.push_back(b);
    }

    UpdateEntities(dt, w, h);
    HandleCollisions(w, h);
    HandleNextWave(w, h);
}

void Game::UpdateEntities(float dt, float w, float h) {
    // bullets
    for (auto& b : bullets) {
        b.position.x += b.velocity.x * dt;
        b.position.y += b.velocity.y * dt;
        b.life -= dt;
        Wrap(b.position, w, h);
    }
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) { return b.life <= 0.0f; }),
        bullets.end()
    );

    // asteroids
    for (auto& a : asteroids) {
        a.position.x += a.velocity.x * dt;
        a.position.y += a.velocity.y * dt;
        Wrap(a.position, w, h);
    }

    // ship wrap (if ship.h doesn’t do it)
    Wrap(ship.position, w, h);
}

void Game::HandleCollisions(float w, float h) {
    // Bullet vs Asteroid
    for (int bi = (int)bullets.size() - 1; bi >= 0; --bi) {
        bool bulletRemoved = false;

        for (int ai = (int)asteroids.size() - 1; ai >= 0; --ai) {
            float dx = bullets[bi].position.x - asteroids[ai].position.x;
            float dy = bullets[bi].position.y - asteroids[ai].position.y;

            dx = WrapDelta(dx, w);
            dy = WrapDelta(dy, h);

            float dist = sqrtf(dx * dx + dy * dy);
            const float bulletRadius = 3.0f;

            if (dist < asteroids[ai].radius + bulletRadius) {
                Asteroid hit = asteroids[ai];

                bullets.erase(bullets.begin() + bi);
                asteroids.erase(asteroids.begin() + ai);
                bulletRemoved = true;

                score += hit.size * 100;

                // split
                if (hit.size > 1) {
                    for (int k = 0; k < 2; k++) {
                        Asteroid child{};
                        child.position = hit.position;
                        child.size = hit.size - 1;
                        child.radius = hit.radius * 0.6f;
                        child.velocity = {
                            hit.velocity.x + (float)GetRandomValue(-120, 120),
                            hit.velocity.y + (float)GetRandomValue(-120, 120)
                        };
                        asteroids.push_back(child);
                    }
                }
                break;
            }
        }

        if (bulletRemoved) continue;
    }

    // Ship vs Asteroid
    if (invulnTimer <= 0.0f) {
        const float shipRadius = 12.0f;

        for (const auto& a : asteroids) {
            float dx = ship.position.x - a.position.x;
            float dy = ship.position.y - a.position.y;

            dx = WrapDelta(dx, w);
            dy = WrapDelta(dy, h);

            float dist = sqrtf(dx * dx + dy * dy);

            if (dist < a.radius + shipRadius) {
                lives--;
                bullets.clear();

                if (lives <= 0) mode = Mode::GameOver;
                else ResetShip(w, h);

                break;
            }
        }
    }
}

void Game::HandleNextWave(float w, float h) {
    if (!asteroids.empty()) return;

    wave++;
    int count = 4 + wave;
    SpawnWave(count, w, h);
    ResetShip(w, h);
}

void Game::ResetShip(float w, float h) {
    ship.position = { w * 0.5f, h * 0.5f };
    ship.velocity = { 0, 0 };
    ship.rotation = 0.0f;
    invulnTimer = invulnDuration;
}

void Game::SpawnWave(int count, float w, float h) {
    asteroids.clear();

    for (int i = 0; i < count; i++) {
        Asteroid a{};
        a.position = { (float)GetRandomValue(0, (int)w), (float)GetRandomValue(0, (int)h) };
        a.velocity = { (float)GetRandomValue(-80, 80), (float)GetRandomValue(-80, 80) };
        a.radius = 40.0f;
        a.size = 3;

        // keep away from center spawn
        float dx = a.position.x - w * 0.5f;
        float dy = a.position.y - h * 0.5f;
        if (fabsf(dx) < 140.0f && fabsf(dy) < 140.0f) {
            a.position.x += (dx < 0 ? -200.0f : 200.0f);
            a.position.y += (dy < 0 ? -150.0f : 150.0f);
            Wrap(a.position, w, h);
        }

        asteroids.push_back(a);
    }
}

void Game::Render(float w, float h) const {
    BeginDrawing();
    ClearBackground(BLACK);

    for (const auto& a : asteroids) {
        DrawCircleLines((int)a.position.x, (int)a.position.y, a.radius, GREEN);
    }

    for (const auto& b : bullets) {
        DrawCircleV(b.position, 3.0f, RED);
    }

    bool shipVisible = true;
    if (invulnTimer > 0.0f) shipVisible = ((int)(invulnTimer * 10.0f) % 2) == 0;

    if (mode == Mode::Playing && shipVisible) ship.Draw();

    DrawText(TextFormat("Score: %i", score), 10, 10, 20, WHITE);
    DrawText(TextFormat("Wave: %i", wave), 10, 35, 20, WHITE);
    DrawText(TextFormat("Lives: %i", lives), 10, 60, 20, WHITE);

    if (mode == Mode::GameOver) {
        DrawText("GAME OVER", (int)(w * 0.5f) - 120, (int)(h * 0.5f) - 40, 40, WHITE);
        DrawText("Press R to Restart", (int)(w * 0.5f) - 120, (int)(h * 0.5f) + 10, 20, WHITE);
    }

    EndDrawing();
}

void Game::Wrap(Vector2& p, float w, float h) {
    if (p.x > w) p.x = 0;
    if (p.x < 0) p.x = w;
    if (p.y > h) p.y = 0;
    if (p.y < 0) p.y = h;
}

float Game::WrapDelta(float d, float size) {
    if (d > size * 0.5f) d -= size;
    if (d < -size * 0.5f) d += size;
    return d;
}
