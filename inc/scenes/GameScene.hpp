#pragma once

#include <raylib.h>

#include <glm/glm.hpp>

#include <cstdint>
#include <vector>

#include "Asteroid.hpp"
#include "Configuration.hpp"
#include "ParticleSystem.hpp"
#include "Player.hpp"
#include "Raycast.hpp"
#include "Scene.hpp"
#include "Starfield.hpp"
#include "Statistics.hpp"

namespace cosmic {

class GameScene : public Scene {
   private:
    static constexpr uint32_t STARFIELD_COUNT = 1000;

    static constexpr float FOV = 70.0F;
    static constexpr float NEAR_PLANE = 0.1F;
    static constexpr float FAR_PLANE = 100.0F;

    static constexpr float BOUNDS_SIZE = 80.0F;

    static constexpr int32_t POINTS_PER_HIT = 10;
    static constexpr int32_t POINTS_PER_DESTROYED_ASTEROID = 100;

   public:
    void Initialize() noexcept override;
    void Shutdown() noexcept override;
    void Update() noexcept override;
    void Render() noexcept override;

   private:
    void DrawGUI() noexcept;
    void DrawCrosshair() noexcept;
    void RenderAsteroids() noexcept;
    void CalculateMatrices() noexcept;

    void LoadAssets() noexcept;
    void UnloadAssets() noexcept;

    void ProcessPlayerMovement() noexcept;
    void ProcessPlayerAttack() noexcept;

    void CheckCollisions() noexcept;
    Asteroid* GetClosestHitAsteroid(const Raycast& ray) noexcept;
    bool IsLevelCleared() noexcept;

    void GenerateAsteroid() noexcept;

    void AdvanceLevel() noexcept;
    void InitializeLevel(uint32_t level) noexcept;

    void SpawnExplosionParticles(glm::vec3 position) noexcept;
    void SpawnHitParticles(glm::vec3 position, float radius) noexcept;

   private:
    // ------ Configuration ------
    Configuration m_config = {};

    // ------ Game Objects ------
    Statistics m_statistics = {};
    Player m_player;
    std::vector<Asteroid> m_asteroids;

    // ------ Graphics ------
    glm::mat4 m_projection;
    glm::mat4 m_view;
    glm::mat4 m_rotation;

    float m_animation_time = 0.0F;

    Starfield m_starfield;
    ParticleSystem m_particle_system;

    // ------ Assets ------
    Sound m_shoot_sound;
    Sound m_destroy_sound;

    Texture2D m_spaceship_tex;
};

}  // namespace cosmic