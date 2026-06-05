#pragma once

#include "Scene.hpp"
#include "Statistics.hpp"

namespace cosmic {

class GameoverScene : public Scene {
   public:
    GameoverScene(const Statistics& stats) noexcept;

    void Initialize() noexcept override;
    void Shutdown() noexcept override;
    void Update() noexcept override;
    void Render() noexcept override;

   private:
    Statistics m_statistics;
};

}  // namespace cosmic