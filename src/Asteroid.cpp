#include "Asteroid.hpp"

#include <cmath>
#include <cstdlib>

#include "Raycast.hpp"

namespace cosmic {

void Asteroid::GenerateVertices(uint32_t vertex_count) noexcept {
    m_vertices.clear();
    m_vertices.reserve(vertex_count);

    // Generate vertices in a triangle strip pattern around a sphere
    // This creates latitude bands that form triangles with consecutive vertices
    uint32_t rings = static_cast<uint32_t>(std::sqrt(vertex_count / 2.0F));
    uint32_t segments = vertex_count / (2 * rings);

    if (rings < 2) rings = 2;
    if (segments < 3) segments = 3;

    for (uint32_t ring = 0; ring < rings && m_vertices.size() < vertex_count; ++ring) {
        float theta1 = static_cast<float>(ring) / static_cast<float>(rings) * M_PIf32;
        float theta2 = static_cast<float>(ring + 1) / static_cast<float>(rings) * M_PIf32;

        for (uint32_t seg = 0; seg < segments && m_vertices.size() < vertex_count; ++seg) {
            float phi = static_cast<float>(seg) / static_cast<float>(segments) * 2.0F * M_PIf32;

            // First vertex of the strip (current ring)
            float x1 = m_radius * std::sin(theta1) * std::cos(phi);
            float y1 = m_radius * std::sin(theta1) * std::sin(phi);
            float z1 = m_radius * std::cos(theta1);
            m_vertices.emplace_back(x1, y1, z1);

            if (m_vertices.size() >= vertex_count) break;

            // Second vertex of the strip (next ring)
            float x2 = m_radius * std::sin(theta2) * std::cos(phi);
            float y2 = m_radius * std::sin(theta2) * std::sin(phi);
            float z2 = m_radius * std::cos(theta2);
            m_vertices.emplace_back(x2, y2, z2);
        }
    }
}

RaycastResult Asteroid::Cast(const Raycast& raycast) const noexcept {
    glm::vec3 to_center = m_position - raycast.origin;
    float projection = glm::dot(to_center, raycast.direction);

    RaycastResult result = {
        .hit = false,
        .distance = 0.0F,
        .position = glm::vec3(0.0F, 0.0F, 0.0F)};

    if (projection < 0.0F) {
        return result;  // Ray is pointing away from asteroid
    }

    float distance_squared = glm::dot(to_center, to_center);
    float perpendicular_distance_squared = distance_squared - projection * projection;

    float radius_squared = m_radius * m_radius;
    if (perpendicular_distance_squared > radius_squared) {
        return result;  // Ray misses
    }

    float closest_distance = std::sqrt(std::max(0.0F, radius_squared - perpendicular_distance_squared));

    float distance = projection - closest_distance;

    result.hit = distance > 0.0F;
    result.distance = distance;
    result.position = raycast.origin + raycast.direction * distance;

    return result;
}

bool Asteroid::CollidesWith(glm::vec3 position, double radius) const noexcept {
    float distance = glm::distance(m_position, position);
    return distance < (m_radius + radius);
}

void Asteroid::ResolveCollision(Asteroid& other, float damping) noexcept {
    glm::vec3 normal = glm::normalize(other.m_position - m_position);
    float relative_speed = glm::dot(other.m_velocity - m_velocity, normal);

    if (relative_speed > 0) return;  // Objects are separating

    float mass1 = GetMass();
    float mass2 = other.GetMass();
    float inverse_mass_sum = 1.0F / mass1 + 1.0F / mass2;

    glm::vec3 impulse = normal * relative_speed * (1.0F + damping) / inverse_mass_sum;

    m_velocity += impulse / mass1;
    other.m_velocity -= impulse / mass2;
}

void Asteroid::ResolveBoundsCollision(glm::vec3 min, glm::vec3 max, float damping) noexcept {
    if (m_position.x - m_radius < min.x) {
        m_position.x = min.x + m_radius;
        m_velocity.x = -m_velocity.x * damping;
    }

    if (m_position.x + m_radius > max.x) {
        m_position.x = max.x - m_radius;
        m_velocity.x = -m_velocity.x * damping;
    }

    if (m_position.y - m_radius < min.y) {
        m_position.y = min.y + m_radius;
        m_velocity.y = -m_velocity.y * damping;
    }

    if (m_position.y + m_radius > max.y) {
        m_position.y = max.y - m_radius;
        m_velocity.y = -m_velocity.y * damping;
    }

    if (m_position.z - m_radius < min.z) {
        m_position.z = min.z + m_radius;
        m_velocity.z = -m_velocity.z * damping;
    }

    if (m_position.z + m_radius > max.z) {
        m_position.z = max.z - m_radius;
        m_velocity.z = -m_velocity.z * damping;
    }
}

float Asteroid::GetMass() noexcept {
    float volume = (4.0F / 3.0F) * M_PIf32 * m_radius * m_radius * m_radius;
    return volume * DENSITY;
}

}  // namespace cosmic