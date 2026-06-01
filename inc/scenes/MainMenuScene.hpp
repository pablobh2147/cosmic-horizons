#pragma once

#include <array>
#include <cstdint>

#include "Scene.hpp"
#include "Starfield.hpp"

namespace cosmic {

class MainMenuScene : public Scene {
   private:
    static constexpr std::array<const char*, 2> MENU_OPTIONS = {"Play", "Quit"};

   public:
    void Initialize() noexcept override;
    void Shutdown() noexcept override;
    void Update() noexcept override;
    void Render() noexcept override;

   private:
    void RenderBackground() noexcept;

    void DrawTitle() noexcept;
    void DrawMenu() noexcept;
    void DrawHint() noexcept;

    void MoveSelectionUp() noexcept;
    void MoveSelectionDown() noexcept;
    void ExecuteSelection() noexcept;

   private:
    int32_t m_selected_option = 0;

    Starfield m_starfield;
    float m_animation_time = 0.0F;
    float m_rotation_angle = 0.0F;
};

}  // namespace cosmic