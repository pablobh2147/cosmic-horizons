#pragma once

#include <cstdint>

#include "Scene.hpp"
#include "Starfield.hpp"

namespace cosmic {

class MainMenuScene : public Scene {
   public:
    void Initialize() noexcept override;
    void Update() noexcept override;
    void Render() noexcept override;
    void Shutdown() noexcept override;

   private:
    int32_t m_selected_option = 0;
    static constexpr int32_t MENU_OPTION_COUNT = 2;

    Starfield m_starfield;
    float m_animation_time = 0.0F;
    float m_rotation_angle = 0.0F;
};

}  // namespace cosmic