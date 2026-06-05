#include "ParticleSystem.hpp"

#include <raylib.h>

#include "Common.hpp"

namespace cosmic {

ParticleSystem::ParticleSystem() noexcept {
    m_particles = new Particle[MAX_PARTICLES];
}

ParticleSystem::~ParticleSystem() noexcept {
    delete[] m_particles;
}

void ParticleSystem::Update(float delta_time) noexcept {
    for (size_t i = 0; i < m_particle_count; ++i) {
        m_particles[i].position += m_particles[i].velocity * delta_time;
        m_particles[i].lifetime -= delta_time;

        if (m_particles[i].lifetime <= 0.0F) {
            m_particles[i] = m_particles[m_particle_count - 1];
            --m_particle_count;
        }
    }
}

void ParticleSystem::Render(const glm::mat4& mvp) noexcept {
    for (size_t i = 0; i < m_particle_count; ++i) {
        glm::vec3 screen_pos = ProjectToScreen(m_particles[i].position, mvp);
        if (screen_pos.z <= 0.0F) continue;

        DrawCircle(static_cast<int32_t>(screen_pos.x), static_cast<int32_t>(screen_pos.y), m_particles[i].size, m_particles[i].color);
    }
}

void ParticleSystem::AddParticle(glm::vec3 position, glm::vec3 velocity, Color color, float size, float lifetime) noexcept {
    if (m_particle_count < MAX_PARTICLES) {
        m_particles[m_particle_count].position = position;
        m_particles[m_particle_count].velocity = velocity;
        m_particles[m_particle_count].color = color;
        m_particles[m_particle_count].lifetime = lifetime;
        m_particles[m_particle_count].size = size;

        m_particle_count++;
    }
}

}  // namespace cosmic