#pragma once

#include <raylib.h>

#include <glm/glm.hpp>

#include <vector>

namespace cosmic {

class Starfield {
   public:
    static constexpr float MIN_TWINKLE_SPEED = 0.5F;
    static constexpr float MAX_TWINKLE_SPEED = 3.0F;
    static constexpr float MIN_BRIGHTNESS = 0.1F;
    static constexpr float MAX_BRIGHTNESS = 0.5F;

   public:
    void Generate(uint32_t count) noexcept;
    void Render(const glm::mat4& mvp, float animation_time) noexcept;

   private:
    struct Star {
        glm::vec3 position;
        Color color;
        float twinkle_speed;
        float twinkle_phase;
        float min_brightness;
    };

   private:
    std::vector<Star> m_starfield;
};

}  // namespace cosmic