#include "Starfield.hpp"

#include "Common.hpp"

namespace cosmic {

void Starfield::Generate(uint32_t count) noexcept {
    constexpr std::array<Color, 5> star_colors = {
        Color {255, 255, 255, 255}, // White
        Color {200, 220, 255, 255}, // Blue-white
        Color {255, 240, 200, 255}, // Warm yellow
        Color {255, 200, 150, 255}, // Orange
        Color {180, 200, 255, 255}, // Cool blue
    };

    m_starfield.reserve(count);
    for (uint32_t i = 0; i < count; ++i) {
        Star& star = m_starfield.emplace_back();
        star.position = RandomVector(-1.0F, 1.0F);
        star.color = star_colors[RandomInt(0, static_cast<int32_t>(star_colors.size()) - 1)];
        star.twinkle_speed = RandomFloat(MIN_TWINKLE_SPEED, MAX_TWINKLE_SPEED);
        star.twinkle_phase = RandomFloat(0.0F, M_2_PIf32);
        star.min_brightness = RandomFloat(MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    }
}

void Starfield::Render(const glm::mat4& mvp, float animation_time) noexcept {
    for (const Star& star : m_starfield) {
        glm::vec3 screen_pos = ProjectToScreen(star.position, mvp);
        if (screen_pos.z <= 0.0F) continue;

        float t = std::sin(animation_time * star.twinkle_speed + star.twinkle_phase);
        float brightness = star.min_brightness + (1.0F - star.min_brightness) * (t * 0.5F + 0.5F);

        Color color = {
            static_cast<unsigned char>(star.color.r * brightness),
            static_cast<unsigned char>(star.color.g * brightness),
            static_cast<unsigned char>(star.color.b * brightness),
            255,
        };

        DrawCircle(static_cast<int>(screen_pos.x), static_cast<int>(screen_pos.y), 1.0F, color);
    }
}

}  // namespace cosmic
