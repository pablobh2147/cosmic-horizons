#include "scenes/GameScene.hpp"

#include <math.h>
#include <raylib.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <cmath>
#include <format>
#include <limits>
#include <span>

#include "Application.hpp"
#include "Asteroid.hpp"
#include "Common.hpp"
#include "scenes/GameoverScene.hpp"

namespace cosmic {

// ----------------------- Scene Methods -----------------------

void GameScene::Initialize() noexcept {
    LoadAssets();
    m_starfield.Generate(STARFIELD_COUNT);

    DisableCursor();
}

void GameScene::Shutdown() noexcept {
    UnloadAssets();

    EnableCursor();
}

void GameScene::Update() noexcept {
    ProcessPlayerMovement();
    ProcessPlayerAttack();

    if (IsKeyPressed(KEY_R)) {
        GenerateAsteroid();
    }

    // Update asteroids
    for (Asteroid& asteroid : m_asteroids) {
        asteroid.Update(GetFrameTime());
    }

    // Remove destroyed asteroids
    std::erase_if(m_asteroids, [](const Asteroid& a) { return a.IsDestroyed(); });

    CheckCollisions();
}

void GameScene::Render() noexcept {
    ClearBackground(BLACK);

    CalculateMatrices();
    m_animation_time += GetFrameTime();

    m_starfield.Render(m_projection * m_rotation, m_animation_time);
    RenderAsteroids();

    DrawGUI();
    DrawCrosshair();
}

// ----------------------- Drawing & Rendering Methods -----------------------

void GameScene::DrawGUI() noexcept {
    DrawFPS(10, 10);

    constexpr uint32_t FONT_SIZE = 50;

    std::string score_str = std::format("Score: {}", m_statistics.score);
    std::string level_str = std::format("Level: {}", m_statistics.level);

    DrawText(score_str.c_str(), GetScreenWidth() * 1 / 4 - MeasureText(score_str.c_str(), FONT_SIZE) / 2, 30, FONT_SIZE, WHITE);
    DrawText(level_str.c_str(), GetScreenWidth() * 3 / 4 - MeasureText(level_str.c_str(), FONT_SIZE) / 2, 30, FONT_SIZE, WHITE);

    DrawText(std::format("Accuracy: {:.1f}%", m_statistics.GetAccuracy() * 100.0F).c_str(), 10, 70, 20, WHITE);

    // Draw lives as spaceship sprites
    for (int i = 0; i < m_player.lives; ++i) {
        constexpr int32_t TEXTURE_SCALE = 4;

        int32_t offset_x = m_spaceship_tex.width * TEXTURE_SCALE / 2;
        int32_t offset_y = m_spaceship_tex.height * TEXTURE_SCALE / 2;

        int32_t x = 50 + i * (m_spaceship_tex.width * TEXTURE_SCALE + 10);
        int32_t y = GetScreenHeight() - 50;

        DrawTextureEx(m_spaceship_tex, {static_cast<float>(x - offset_x), static_cast<float>(y - offset_y)}, 0.0F, TEXTURE_SCALE, WHITE);
    }
}

void GameScene::DrawCrosshair() noexcept {
    constexpr Color CROSSHAIR_COLOR = {255, 255, 255, 255};
    constexpr int CROSSHAIR_SIZE = 10;

    int center_x = GetScreenWidth() / 2;
    int center_y = GetScreenHeight() / 2;

    DrawLine(center_x - CROSSHAIR_SIZE, center_y, center_x + CROSSHAIR_SIZE, center_y, CROSSHAIR_COLOR);
    DrawLine(center_x, center_y - CROSSHAIR_SIZE, center_x, center_y + CROSSHAIR_SIZE, CROSSHAIR_COLOR);
}

void GameScene::RenderAsteroids() noexcept {
    glm::mat4 view_projection = m_projection * m_view;

    Asteroid* closest_asteroid = GetClosestHitAsteroid(m_player.GetRaycast());

    for (const Asteroid& asteroid : m_asteroids) {
        glm::mat4 model = glm::mat4(1.0F);
        model = glm::translate(model, asteroid.GetPosition());
        model = glm::rotate(model, asteroid.GetRotation().x, glm::vec3(1.0F, 0.0F, 0.0F));
        model = glm::rotate(model, asteroid.GetRotation().y, glm::vec3(0.0F, 1.0F, 0.0F));

        glm::mat4 mvp = view_projection * model;
        Color color = &asteroid == closest_asteroid ? GREEN : LIGHTGRAY;

        if (!asteroid.IsDestroyed()) {
            RenderPoligon(asteroid.GetVertices(), mvp, color);
        }
    }
}

void GameScene::CalculateMatrices() noexcept {
    float aspect_ratio = static_cast<float>(GetScreenWidth()) / static_cast<float>(GetScreenHeight());
    m_projection = glm::perspective(glm::radians(FOV), aspect_ratio, NEAR_PLANE, FAR_PLANE);

    m_rotation = glm::mat4_cast(glm::conjugate(m_player.m_orientation));
    m_view = m_rotation * glm::translate(glm::mat4(1.0F), -m_player.m_position);
}

// ----------------------- Asset Management Methods -----------------------

void GameScene::LoadAssets() noexcept {
    m_shoot_sound = LoadSound("assets/sounds/shoot.mp3");
    m_destroy_sound = LoadSound("assets/sounds/destroy.mp3");

    Image img = LoadImage("assets/textures/spaceship.png");
    m_spaceship_tex = LoadTextureFromImage(img);
    UnloadImage(img);
}

void GameScene::UnloadAssets() noexcept {
    UnloadSound(m_shoot_sound);
    UnloadSound(m_destroy_sound);

    UnloadTexture(m_spaceship_tex);
}

// ----------------------- Player Input -----------------------

void GameScene::ProcessPlayerMovement() noexcept {
    constexpr float ROTATION_SPEED = 120.0F;
    constexpr float ROLL_SPEED = 2.0F;

    float mov_delta = m_player.speed * GetFrameTime();
    float rot_delta = ROTATION_SPEED * GetFrameTime();
    float roll_delta = ROLL_SPEED * GetFrameTime();

    glm::vec3 forward = m_player.GetForward();
    glm::vec3 right = m_player.GetRight();
    glm::vec3 up = m_player.GetUp();

    float yaw_angle = -(GetMouseDelta().x / GetScreenWidth()) * rot_delta * M_2_PIf32;
    float pitch_angle = -(GetMouseDelta().y / GetScreenHeight()) * rot_delta * M_2_PIf32;

    glm::quat q_yaw = glm::angleAxis(yaw_angle, up);
    glm::quat q_pitch = glm::angleAxis(pitch_angle, right);
    m_player.m_orientation = glm::normalize(q_yaw * q_pitch * m_player.m_orientation);

    m_player.m_position += forward * mov_delta;

    if (IsKeyDown(m_config.input.up_key)) {
        m_player.m_position += up * mov_delta;
        glm::quat q_pitch = glm::angleAxis(roll_delta, right);
        m_player.m_orientation = glm::normalize(q_pitch * m_player.m_orientation);
    }

    if (IsKeyDown(m_config.input.down_key)) {
        m_player.m_position -= up * mov_delta;
        glm::quat q_pitch = glm::angleAxis(-roll_delta, right);
        m_player.m_orientation = glm::normalize(q_pitch * m_player.m_orientation);
    }

    if (IsKeyDown(m_config.input.roll_left_key)) {
        m_player.m_position -= right * mov_delta;
        glm::quat q_roll = glm::angleAxis(-roll_delta, forward);
        m_player.m_orientation = glm::normalize(q_roll * m_player.m_orientation);
    }

    if (IsKeyDown(m_config.input.roll_right_key)) {
        m_player.m_position += right * mov_delta;
        glm::quat q_roll = glm::angleAxis(roll_delta, forward);
        m_player.m_orientation = glm::normalize(q_roll * m_player.m_orientation);
    }
}

void GameScene::ProcessPlayerAttack() noexcept {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        m_statistics.shots_fired++;
        PlaySound(m_shoot_sound);

        Asteroid* hit_asteroid = GetClosestHitAsteroid(m_player.GetRaycast());

        if (hit_asteroid != nullptr) {
            hit_asteroid->Attack();
            m_statistics.score += POINTS_PER_HIT;

            if (hit_asteroid->IsDestroyed()) {
                m_statistics.asteroids_destroyed++;
                m_statistics.score += POINTS_PER_DESTROYED_ASTEROID;
                PlaySound(m_destroy_sound);
            }
        } else {
            m_statistics.misses++;
        }
    }
}

// ----------------------- Physics -----------------------

void GameScene::CheckCollisions() noexcept {
    // Check collisions between asteroids and player
    for (auto it = m_asteroids.begin(); it != m_asteroids.end(); ++it) {
        if (it->CollidesWith(m_player)) {
            m_player.lives -= 1;

            if (m_player.lives <= 0) {
                GetApplication()->TransitionScene<GameoverScene>();
                return;
            }

            it = m_asteroids.erase(it);
            if (it == m_asteroids.end()) {
                break;
            }

            PlaySound(m_destroy_sound);
        }
    }

    // Check collisions between asteroids and resolve boundary collisions
    for (auto it = m_asteroids.begin(); it != m_asteroids.end(); ++it) {
        for (auto jt = it + 1; jt != m_asteroids.end(); ++jt) {
            if (it->CollidesWith(*jt)) {
                it->ResolveCollision(*jt, m_config.physics.damping);
            }
        }

        it->ResolveBoundsCollision(glm::vec3(-BOUNDS_SIZE), glm::vec3(BOUNDS_SIZE), m_config.physics.damping);
    }
}

Asteroid* GameScene::GetClosestHitAsteroid(const Raycast& ray) noexcept {
    float closest_distance = std::numeric_limits<float>::max();
    Asteroid* hit_asteroid = nullptr;

    for (Asteroid& asteroid : m_asteroids) {
        RaycastResult result = asteroid.Cast(ray);
        if (result.hit && result.distance < closest_distance) {
            closest_distance = result.distance;
            hit_asteroid = &asteroid;
        }
    }

    return hit_asteroid;
}

// ----------------------- Generation -----------------------

void GameScene::GenerateAsteroid() noexcept {
    constexpr float MIN_RADIUS = 1.0F;
    constexpr float MAX_RADIUS = 12.0F;

    constexpr float LIVES_PER_RADIUS = 0.5F;
    constexpr uint32_t VERTEX_COUNT_PER_RADIUS = 5;
    constexpr uint32_t MIN_VERTEX_COUNT = 10;

    constexpr float MIN_VELOCITY = 5.0F;
    constexpr float MAX_VELOCITY = 40.0F;
    constexpr float MIN_ANGULAR_VELOCITY = 0.5F;
    constexpr float MAX_ANGULAR_VELOCITY = 2.0F;

    Asteroid& asteroid = m_asteroids.emplace_back();
    float base_radius = RandomFloat(MIN_RADIUS, MAX_RADIUS);
    uint32_t lives = static_cast<uint32_t>(base_radius * LIVES_PER_RADIUS) + 1;
    uint32_t vertex_count = static_cast<uint32_t>(base_radius * VERTEX_COUNT_PER_RADIUS) + MIN_VERTEX_COUNT;

    asteroid.SetRadius(base_radius);
    asteroid.SetLives(lives);
    asteroid.GenerateVertices(vertex_count);

    asteroid.SetPosition(RandomVector(-BOUNDS_SIZE * 0.8F, BOUNDS_SIZE * 0.8F));
    asteroid.SetRotation(RandomVector(0.0F, M_2_PIf32));

    glm::vec3 velocity_direction = glm::normalize(RandomVector(-1.0F, 1.0F));
    glm::vec3 angular_velocity_direction = glm::normalize(RandomVector(-1.0F, 1.0F));

    asteroid.SetVelocity(velocity_direction * RandomFloat(MIN_VELOCITY, MAX_VELOCITY));
    asteroid.SetAngularVelocity(angular_velocity_direction * RandomFloat(MIN_ANGULAR_VELOCITY, MAX_ANGULAR_VELOCITY));
}

}  // namespace cosmic
