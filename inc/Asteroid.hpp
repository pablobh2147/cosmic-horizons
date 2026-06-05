#pragma once

#include <glm/glm.hpp>

#include <vector>

#include "Player.hpp"
#include "Raycast.hpp"

namespace cosmic {

class Asteroid {
   private:
    static constexpr float DENSITY = 1.0F;
    static constexpr float SHRINK_FACTOR = 0.7;

   public:
    Asteroid() = default;
    ~Asteroid() = default;

    Asteroid(const Asteroid&) = default;
    Asteroid(Asteroid&&) noexcept = default;

    Asteroid& operator=(const Asteroid&) = default;
    Asteroid& operator=(Asteroid&&) noexcept = default;

    void GenerateVertices(uint32_t vertex_count) noexcept;

    [[nodiscard]] const std::vector<glm::vec3>& GetVertices() const noexcept { return m_vertices; }

    [[nodiscard]] glm::vec3 GetPosition() const noexcept { return m_position; }
    [[nodiscard]] glm::vec3 GetVelocity() const noexcept { return m_velocity; }
    [[nodiscard]] glm::vec3 GetRotation() const noexcept { return m_rotation; }
    [[nodiscard]] glm::vec3 GetAngularVelocity() const noexcept { return m_angular_velocity; }
    [[nodiscard]] float GetRadius() const noexcept { return m_radius; }
    [[nodiscard]] uint32_t GetLives() const noexcept { return m_lives; }

    void SetPosition(const glm::vec3& position) noexcept { m_position = position; }
    void SetVelocity(const glm::vec3& velocity) noexcept { m_velocity = velocity; }
    void SetRotation(const glm::vec3& rotation) noexcept { m_rotation = rotation; }
    void SetAngularVelocity(const glm::vec3& angular_velocity) noexcept { m_angular_velocity = angular_velocity; }
    void SetRadius(float radius) noexcept { m_radius = radius; }
    void SetLives(uint32_t lives) noexcept { m_lives = lives; }

    bool IsDestroyed() const noexcept { return m_lives <= 0; }

    void Attack() noexcept {
        if (m_lives > 0) {
            m_radius *= SHRINK_FACTOR;
            int32_t vertices = static_cast<int32_t>(m_vertices.size() * SHRINK_FACTOR);
            if (vertices > 0) {
                GenerateVertices(vertices);
            }
            m_lives--;
        }
    }

    void Move(const glm::vec3& delta) noexcept { m_position += delta; }
    void Rotate(const glm::vec3& delta) noexcept { m_rotation += delta; }

    void Update(float delta_time) noexcept {
        Move(m_velocity * delta_time);
        Rotate(m_angular_velocity * delta_time);
    }

    RaycastResult Cast(const Raycast& raycast) const noexcept;

    bool CollidesWith(glm::vec3 position, float radius) const noexcept;
    bool CollidesWith(const Asteroid& other) const noexcept { return CollidesWith(other.m_position, other.m_radius); }
    bool CollidesWith(const Player& player) const noexcept { return CollidesWith(player.m_position, Player::COLLISION_RADIUS); }

    void ResolveCollision(Asteroid& other, float damping) noexcept;
    void ResolveBoundsCollision(glm::vec3 min, glm::vec3 max, float damping) noexcept;

   private:
    float GetMass() noexcept;

   private:
    glm::vec3 m_position = {0.0F, 0.0F, 0.0F};
    glm::vec3 m_velocity = {0.0F, 0.0F, 0.0F};
    glm::vec3 m_rotation = {0.0F, 0.0F, 0.0F};
    glm::vec3 m_angular_velocity = {0.0F, 0.0F, 0.0F};

    float m_radius = 1.0;

    uint32_t m_lives = 3;

    std::vector<glm::vec3> m_vertices;
};

}  // namespace cosmic
