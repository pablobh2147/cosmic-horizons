#pragma once

#include <raylib.h>

#include <glm/glm.hpp>

#include <cstdint>
#include <vector>

#include "Asteroid.hpp"
#include "Configuration.hpp"
#include "Player.hpp"
#include "Scene.hpp"

namespace cosmic {

struct Statistics {
    int32_t asteroids_destroyed = 0;
    int32_t score = 0;
    int32_t level = 1;

    int32_t shots_fired = 0;
    int32_t misses = 0;

    float GetAccuracy() const noexcept {
        if (shots_fired == 0) {
            return 0.0F;
        }
        return 1.0F - static_cast<float>(misses) / static_cast<float>(shots_fired);
    }
};

class GameScene : public Scene {
   private:
    static constexpr uint32_t STARFIELD_COUNT = 1000;
    static constexpr float MIN_TWINKLE_SPEED = 0.5F;
    static constexpr float MAX_TWINKLE_SPEED = 3.0F;
    static constexpr float MIN_BRIGHTNESS = 0.1F;
    static constexpr float MAX_BRIGHTNESS = 0.5F;

   public:
    void Initialize() noexcept override;
    void Update() noexcept override;
    void Render() noexcept override;
    void Shutdown() noexcept override;

   private:
    void DrawGUI() noexcept;
    void DrawCrosshair() noexcept;
    void DrawStarfield() noexcept;

    void LoadAssets() noexcept;
    void UnloadAssets() noexcept;

    void ProcessPlayerMovement() noexcept;
    void CheckCollisions() noexcept;

    void GenerateStarfield() noexcept;

    void CalculateMatrices() noexcept;

    void GenerateAsteroid() noexcept;

   private:
    struct Star {
        glm::vec3 position;
        Color color;
        float twinkle_speed;
        float twinkle_phase;
        float min_brightness;
    };

   private:
    Configuration m_config = {};

    Player m_player;
    std::vector<Asteroid> m_asteroids;
    Statistics m_statistics;
    BoundingBox m_bounds;

    glm::mat4 m_projection;
    glm::mat4 m_view;
    glm::mat4 m_rotation;

    float m_animation_time = 0.0F;
    std::vector<Star> m_starfield;

    // Assets
    Sound m_shoot_sound;
    Sound m_destroy_sound;
};

}  // namespace cosmic