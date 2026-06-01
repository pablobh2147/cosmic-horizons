#pragma once

#include "Scene.hpp"

namespace cosmic {

class GameoverScene : public Scene {
   public:
    void Initialize() noexcept override;
    void Shutdown() noexcept override;
    void Update() noexcept override;
    void Render() noexcept override;
};

}  // namespace cosmic