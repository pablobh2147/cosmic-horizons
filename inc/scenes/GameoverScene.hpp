#pragma once

#include "Scene.hpp"

namespace cosmic {

class GameoverScene : public Scene {
   public:
    GameoverScene() = default;
    ~GameoverScene() = default;

    void Initialize() noexcept override;
    void Update() noexcept override;
    void Render() noexcept override;
    void Shutdown() noexcept override;
};

}  // namespace cosmic