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

void Asteroid::ResolveCollision(Asteroid& other) noexcept {
    // Simple elastic collision response
    glm::vec3 normal = glm::normalize(other.m_position - m_position);
    float relative_speed = glm::dot(other.m_velocity - m_velocity, normal);

    if (relative_speed > 0) return;  // Objects separating

    // Swap velocities (simplified elastic collision)
    glm::vec3 temp_velocity = m_velocity;
    m_velocity = other.m_velocity;
    other.m_velocity = temp_velocity;
}

void Asteroid::ResolveBoundsCollision(const BoundingBox& bounds) noexcept {
    constexpr float BOUNCE_DAMPING = 1.0F;

    if (m_position.x - m_radius < bounds.min.x) {
        m_position.x = bounds.min.x + m_radius;
        m_velocity.x = -m_velocity.x * BOUNCE_DAMPING;
    }

    if (m_position.x + m_radius > bounds.max.x) {
        m_position.x = bounds.max.x - m_radius;
        m_velocity.x = -m_velocity.x * BOUNCE_DAMPING;
    }

    if (m_position.y - m_radius < bounds.min.y) {
        m_position.y = bounds.min.y + m_radius;
        m_velocity.y = -m_velocity.y * BOUNCE_DAMPING;
    }

    if (m_position.y + m_radius > bounds.max.y) {
        m_position.y = bounds.max.y - m_radius;
        m_velocity.y = -m_velocity.y * BOUNCE_DAMPING;
    }

    if (m_position.z - m_radius < bounds.min.z) {
        m_position.z = bounds.min.z + m_radius;
        m_velocity.z = -m_velocity.z * BOUNCE_DAMPING;
    }

    if (m_position.z + m_radius > bounds.max.z) {
        m_position.z = bounds.max.z - m_radius;
        m_velocity.z = -m_velocity.z * BOUNCE_DAMPING;
    }
}

}  // namespace cosmic