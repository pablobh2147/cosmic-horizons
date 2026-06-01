#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <cstdint>

#include "Raycast.hpp"

namespace cosmic {

struct Player {
    static constexpr float COLLISION_RADIUS = 1.2F;
    static constexpr float MIN_SPEED = 10.0F;
    static constexpr float MAX_SPEED = 100.0F;

    static constexpr int32_t MAX_LIVES = 3;
    static constexpr int32_t INITIAL_LIVES = 3;

    static constexpr float INITIAL_SPEED = 20.0F;

    glm::vec3 m_position = {0.0F, 0.0F, 0.0F};
    glm::quat m_orientation = glm::quat(1.0F, 0.0F, 0.0F, 0.0F);

    float speed = INITIAL_SPEED;
    int32_t lives = INITIAL_LIVES;

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