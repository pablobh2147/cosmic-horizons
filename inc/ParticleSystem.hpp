#pragma once

#include <raylib.h>

#include <glm/glm.hpp>

#include <cstddef>

namespace cosmic {

struct Particle {
    glm::vec3 position = glm::vec3(0.0F);
    glm::vec3 velocity = glm::vec3(0.0F);
    Color color = {0, 0, 0, 0};
    float size = 0.0F;
    float lifetime = 0.0F;
};

class ParticleSystem {
   private:
    static constexpr size_t MAX_PARTICLES = 1000;

   private:
    Particle* m_particles;
    size_t m_particle_count = 0;

   public:
    ParticleSystem() noexcept;
    ~ParticleSystem() noexcept;

    void Update(float delta_time) noexcept;
    void Render(const glm::mat4& mvp) noexcept;

    void AddParticle(glm::vec3 position, glm::vec3 velocity, Color color, float size, float lifetime) noexcept;
};

}  // namespace cosmic