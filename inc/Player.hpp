#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <cstdint>

#include "Raycast.hpp"

namespace cosmic {

class Player {
   public:
    static constexpr float COLLISION_RADIUS = 1.2F;
    static constexpr float MIN_SPEED = 10.0F;
    static constexpr float MAX_SPEED = 100.0F;

   public:
    glm::vec3 m_position = {0.0F, 0.0F, 0.0F};
    glm::quat m_orientation = glm::quat(1.0F, 0.0F, 0.0F, 0.0F);

    float speed = 20.0F;
    float rotation_speed = 120.0F;

    int32_t health = 100;

   public:
    glm::vec3 GetForward() const {
        return glm::normalize(m_orientation * glm::vec3(0.0F, 0.0F, -1.0F));
    }

    glm::vec3 GetRight() const {
        return glm::normalize(m_orientation * glm::vec3(1.0F, 0.0F, 0.0F));
    }

    glm::vec3 GetUp() const {
        return glm::normalize(m_orientation * glm::vec3(0.0F, 1.0F, 0.0F));
    }

    Raycast GetRaycast() const {
        return Raycast(m_position, GetForward());
    }
};

}  // namespace cosmic