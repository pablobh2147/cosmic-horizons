#include "scenes/GameScene.hpp"

#include <math.h>
#include <raylib.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <format>
#include <limits>
#include <span>

#include "Asteroid.hpp"

namespace cosmic {

glm::vec3 ProjectToScreen(const glm::vec3& pos, const glm::mat4& mvp) noexcept {
    glm::vec4 projected_pos = mvp * glm::vec4(pos, 1.0F);

    float x = ((projected_pos.x / projected_pos.w) * 0.5F + 0.5F) * GetScreenWidth();
    float y = (1.0F - ((projected_pos.y / projected_pos.w) * 0.5F + 0.5F)) * GetScreenHeight();

    return glm::vec3(x, y, projected_pos.w);
}

void RenderPoint(const glm::vec3& pos, const glm::mat4& mvp, Color color) noexcept {
    glm::vec3 screen_pos = ProjectToScreen(pos, mvp);
    if (screen_pos.z <= 0.0F) return;
    DrawCircle(static_cast<int>(screen_pos.x), static_cast<int>(screen_pos.y), 2, color);
}

void RenderTriangle(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, Color color) noexcept {
    if (p0.z <= 0.0F || p1.z <= 0.0F || p2.z <= 0.0F) return;

    DrawLine(static_cast<int>(p0.x), static_cast<int>(p0.y), static_cast<int>(p1.x), static_cast<int>(p1.y), color);
    DrawLine(static_cast<int>(p1.x), static_cast<int>(p1.y), static_cast<int>(p2.x), static_cast<int>(p2.y), color);
    DrawLine(static_cast<int>(p2.x), static_cast<int>(p2.y), static_cast<int>(p0.x), static_cast<int>(p0.y), color);
}

void RenderTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::mat4& mvp, Color color) noexcept {
    glm::vec3 p0 = ProjectToScreen(v0, mvp);
    glm::vec3 p1 = ProjectToScreen(v1, mvp);
    glm::vec3 p2 = ProjectToScreen(v2, mvp);

    RenderTriangle(p0, p1, p2, color);
}

float RandomFloat(float min, float max) noexcept {
    return min + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (max - min);
}

int32_t RandomInt(int32_t min, int32_t max) noexcept {
    return min + std::rand() % (max - min + 1);
}

glm::vec3 RandomVector(float min, float max) {
    return glm::vec3 {
        RandomFloat(min, max),
        RandomFloat(min, max),
        RandomFloat(min, max),
    };
}

constexpr std::array<glm::vec3, 7> triangle_vertices = {
    glm::vec3 {-1.0F, -1.0F, 0.0F},
    glm::vec3 {1.0F,  -1.0F, 0.0F},
    glm::vec3 {0.0F,  1.0F,  0.0F},
    glm::vec3 {-1.0F, 1.0F,  0.0F},
    glm::vec3 {1.0F,  1.0F,  0.0F},
    glm::vec3 {-0.5F, -0.5F, 1.0F},
    glm::vec3 {0.5F,  -0.5F, 1.0F},
};

void GameScene::Initialize() noexcept {
    constexpr float BOUNDS_SIZE = 80.0F;

    m_bounds = {
        .min = {-BOUNDS_SIZE, -BOUNDS_SIZE, -BOUNDS_SIZE},
        .max = {BOUNDS_SIZE,  BOUNDS_SIZE,  BOUNDS_SIZE },
    };

    LoadAssets();
    GenerateStarfield();

    DisableCursor();
}

void GameScene::Shutdown() noexcept {
    UnloadAssets();

    EnableCursor();
}

void GameScene::GenerateAsteroid() noexcept {
    Asteroid& asteroid = m_asteroids.emplace_back();
    float base_radius = RandomFloat(1.0F, 12.0F);
    uint32_t lives = static_cast<uint32_t>(base_radius * 0.5F) + 1;
    uint32_t vertex_count = static_cast<uint32_t>(base_radius * 5.0F) + 10;

    asteroid.SetRadius(base_radius);
    asteroid.SetLives(lives);
    asteroid.GenerateVertices(vertex_count);

    asteroid.SetPosition(RandomVector(-10.0F, 10.0F));
    asteroid.SetRotation(RandomVector(0.0F, M_2_PIf32));

    glm::vec3 velocity_direction = glm::normalize(RandomVector(-1.0F, 1.0F));
    glm::vec3 angular_velocity_direction = glm::normalize(RandomVector(-1.0F, 1.0F));

    asteroid.SetVelocity(velocity_direction * RandomFloat(5.0F, 40.0F));
    asteroid.SetAngularVelocity(angular_velocity_direction * RandomFloat(0.5F, 2.0F));
}

void GameScene::GenerateStarfield() noexcept {
    constexpr std::array<Color, 5> star_colors = {
        Color {255, 255, 255, 255}, // White
        Color {200, 220, 255, 255}, // Blue-white
        Color {255, 240, 200, 255}, // Warm yellow
        Color {255, 200, 150, 255}, // Orange
        Color {180, 200, 255, 255}, // Cool blue
    };

    m_starfield.reserve(STARFIELD_COUNT);
    for (uint32_t i = 0; i < STARFIELD_COUNT; ++i) {
        Star& star = m_starfield.emplace_back();
        star.position = RandomVector(-1.0F, 1.0F);
        star.color = star_colors[RandomInt(0, static_cast<int32_t>(star_colors.size()) - 1)];
        star.twinkle_speed = RandomFloat(MIN_TWINKLE_SPEED, MAX_TWINKLE_SPEED);
        star.twinkle_phase = RandomFloat(0.0F, M_2_PIf32);
        star.min_brightness = RandomFloat(MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    }
}

void GameScene::LoadAssets() noexcept {
    m_shoot_sound = LoadSound("assets/sounds/shoot.mp3");
    m_destroy_sound = LoadSound("assets/sounds/destroy.mp3");
}

void GameScene::UnloadAssets() noexcept {
    UnloadSound(m_shoot_sound);
    UnloadSound(m_destroy_sound);
}

void GameScene::Update() noexcept {
    ProcessPlayerMovement();

    if (IsKeyPressed(KEY_R)) {
        GenerateAsteroid();
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        m_statistics.shots_fired++;
        PlaySound(m_shoot_sound);

        Raycast ray = m_player.GetRaycast();
        float closest_distance = std::numeric_limits<float>::max();
        Asteroid* hit_asteroid = nullptr;

        for (Asteroid& asteroid : m_asteroids) {
            RaycastResult result = asteroid.Cast(ray);
            if (result.hit && result.distance < closest_distance) {
                closest_distance = result.distance;
                hit_asteroid = &asteroid;
            }
        }

        if (hit_asteroid != nullptr) {
            hit_asteroid->Attack();
            m_statistics.score += 10;

            if (hit_asteroid->IsDestroyed()) {
                m_statistics.asteroids_destroyed++;
                m_statistics.score += 100;
                PlaySound(m_destroy_sound);
            }
        } else {
            m_statistics.misses++;
        }
    }

    m_animation_time += GetFrameTime();

    for (Asteroid& asteroid : m_asteroids) {
        asteroid.Update(GetFrameTime());
    }

    // Remove destroyed asteroids
    std::erase_if(m_asteroids, [](const Asteroid& a) { return a.IsDestroyed(); });

    CheckCollisions();
}

void GameScene::CheckCollisions() noexcept {
    for (auto it = m_asteroids.begin(); it != m_asteroids.end(); ++it) {
        if (it->CollidesWith(m_player)) {
            m_player.health -= 10;

            it = m_asteroids.erase(it);
            if (it == m_asteroids.end()) {
                break;
            }

            PlaySound(m_destroy_sound);
            continue;
        }
    }

    for (size_t i = 0; i < m_asteroids.size(); ++i) {
        for (size_t j = i + 1; j < m_asteroids.size(); ++j) {
            if (m_asteroids[i].CollidesWith(m_asteroids[j])) {
                m_asteroids[i].ResolveCollision(m_asteroids[j]);
            }
        }

        m_asteroids[i].ResolveBoundsCollision(m_bounds);
    }
}

void GameScene::ProcessPlayerMovement() noexcept {
    float mov_delta = m_player.speed * GetFrameTime();
    float rot_delta = m_player.rotation_speed * GetFrameTime();

    glm::vec3 forward = m_player.GetForward();
    glm::vec3 right = m_player.GetRight();
    glm::vec3 up = m_player.GetUp();

    float yaw_angle = -(GetMouseDelta().x / GetScreenWidth()) * rot_delta;
    float pitch_angle = -(GetMouseDelta().y / GetScreenHeight()) * rot_delta;

    glm::quat q_yaw = glm::angleAxis(yaw_angle, up);
    glm::quat q_pitch = glm::angleAxis(pitch_angle, right);
    m_player.m_orientation = glm::normalize(q_yaw * q_pitch * m_player.m_orientation);

    m_player.m_position += forward * mov_delta;

    if (IsKeyDown(m_config.input.up_key)) {
        // m_player.m_position += up * mov_delta;
        // glm::quat q_pitch = glm::angleAxis(rot_delta * 0.01F, right);
        // m_player.m_orientation = glm::normalize(q_pitch * m_player.m_orientation);
    }

    if (IsKeyDown(m_config.input.down_key)) {
        // m_player.m_position -= up * mov_delta;
        // glm::quat q_pitch = glm::angleAxis(-rot_delta * 0.01F, right);
        // m_player.m_orientation = glm::normalize(q_pitch * m_player.m_orientation);
    }

    if (IsKeyDown(m_config.input.roll_left_key)) {
        m_player.m_position -= right * mov_delta;
        glm::quat q_roll = glm::angleAxis(-rot_delta * 0.01F, forward);
        m_player.m_orientation = glm::normalize(q_roll * m_player.m_orientation);
    }

    if (IsKeyDown(m_config.input.roll_right_key)) {
        m_player.m_position += right * mov_delta;
        glm::quat q_roll = glm::angleAxis(rot_delta * 0.01F, forward);
        m_player.m_orientation = glm::normalize(q_roll * m_player.m_orientation);
    }

    float wheel_move = GetMouseWheelMove();
    if (wheel_move != 0.0F) {
        m_player.speed = std::clamp(m_player.speed + wheel_move, Player::MIN_SPEED, Player::MAX_SPEED);
    }
}

void RenderPoligon(std::span<const glm::vec3> vertices, const glm::mat4& mvp, Color color) noexcept {
    if (vertices.size() < 3) {
        return;
    }

    glm::vec3 v0;
    glm::vec3 v1 = ProjectToScreen(vertices[0], mvp);
    glm::vec3 v2 = ProjectToScreen(vertices[1], mvp);

    for (size_t i = 2; i < vertices.size(); ++i) {
        v0 = v1;
        v1 = v2;
        v2 = ProjectToScreen(vertices[i], mvp);

        RenderTriangle(v0, v1, v2, color);
    }
}

void RenderAsteroid(const Asteroid& asteroid, const glm::mat4& mvp, Color color) noexcept {
    if (!asteroid.IsDestroyed()) {
        const std::vector<glm::vec3>& vertices = asteroid.GetVertices();
        RenderPoligon(vertices, mvp, color);
    }
}

void GameScene::Render() noexcept {
    ClearBackground(BLACK);

    DrawGUI();
    DrawCrosshair();

    CalculateMatrices();
    glm::mat4 view_projection = m_projection * m_view;

    RenderTriangle(triangle_vertices[0], triangle_vertices[1], triangle_vertices[2], view_projection, RED);

    // Render asteroids
    Raycast player_ray = m_player.GetRaycast();

    for (const Asteroid& asteroid : m_asteroids) {
        glm::mat4 model = glm::mat4(1.0F);
        model = glm::translate(model, asteroid.GetPosition());
        model = glm::rotate(model, asteroid.GetRotation().x, glm::vec3(1.0F, 0.0F, 0.0F));
        model = glm::rotate(model, asteroid.GetRotation().y, glm::vec3(0.0F, 1.0F, 0.0F));

        glm::mat4 mvp = view_projection * model;

        Color color = LIGHTGRAY;

        if (asteroid.Cast(player_ray).hit) {
            color = GREEN;
        }

        RenderAsteroid(asteroid, mvp, color);
    }

    DrawStarfield();
}

void GameScene::DrawGUI() noexcept {
    DrawFPS(10, 10);
    DrawText("Cosmic Horizons", GetScreenWidth() / 2 - MeasureText("Cosmic Horizons", 50) / 2, 30, 50, WHITE);
    DrawText(std::format("Asteroids Destroyed: {}", m_statistics.asteroids_destroyed).c_str(), 10, 50, 20, WHITE);
    DrawText(std::format("Score: {}", m_statistics.score).c_str(), 10, 70, 20, WHITE);
    DrawText(std::format("Level: {}", m_statistics.level).c_str(), 10, 90, 20, WHITE);
    DrawText(std::format("Accuracy: {:.1f}%", m_statistics.GetAccuracy() * 100.0F).c_str(), 10, 110, 20, WHITE);
    DrawText(std::format("Health: {}", m_player.health).c_str(), 10, 130, 20, RED);
    DrawText(std::format("Shots Fired: {}", m_statistics.shots_fired).c_str(), 10, 150, 20, WHITE);
}

void GameScene::DrawCrosshair() noexcept {
    constexpr Color CROSSHAIR_COLOR = {255, 255, 255, 255};
    constexpr int CROSSHAIR_SIZE = 10;

    int center_x = GetScreenWidth() / 2;
    int center_y = GetScreenHeight() / 2;

    DrawLine(center_x - CROSSHAIR_SIZE, center_y, center_x + CROSSHAIR_SIZE, center_y, CROSSHAIR_COLOR);
    DrawLine(center_x, center_y - CROSSHAIR_SIZE, center_x, center_y + CROSSHAIR_SIZE, CROSSHAIR_COLOR);
}

void GameScene::DrawStarfield() noexcept {
    for (const Star& star : m_starfield) {
        glm::vec3 screen_pos = ProjectToScreen(star.position, m_projection * m_rotation);
        if (screen_pos.z <= 0.0F) continue;

        float t = std::sin(m_animation_time * star.twinkle_speed + star.twinkle_phase);
        float brightness = star.min_brightness + (1.0F - star.min_brightness) * (t * 0.5F + 0.5F);

        Color color = {
            static_cast<unsigned char>(star.color.r * brightness),
            static_cast<unsigned char>(star.color.g * brightness),
            static_cast<unsigned char>(star.color.b * brightness),
            255,
        };

        float radius = 0.5F + brightness * 1.0F;
        DrawCircle(static_cast<int>(screen_pos.x), static_cast<int>(screen_pos.y), radius, color);
    }
}

void GameScene::CalculateMatrices() noexcept {
    constexpr float FOV = 70.0F;
    constexpr float NEAR_PLANE = 0.1F;
    constexpr float FAR_PLANE = 100.0F;

    float aspect_ratio = static_cast<float>(GetScreenWidth()) / static_cast<float>(GetScreenHeight());
    m_projection = glm::perspective(glm::radians(FOV), aspect_ratio, NEAR_PLANE, FAR_PLANE);

    m_rotation = glm::mat4_cast(glm::conjugate(m_player.m_orientation));
    m_view = m_rotation * glm::translate(glm::mat4(1.0F), -m_player.m_position);
}

}  // namespace cosmic
