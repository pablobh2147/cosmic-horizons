#pragma once

#include <glm/glm.hpp>

namespace cosmic {

struct Raycast {
    glm::vec3 origin;
    glm::vec3 direction;

    Raycast() : origin(0.0F, 0.0F, 0.0F), direction(0.0F, 0.0F, 0.0F) {}
    Raycast(const glm::vec3& origin, const glm::vec3& direction) : origin(origin), direction(direction) {}
};

struct RaycastResult {
    bool hit;
    float distance;
    glm::vec3 position;
};

}  // namespace cosmic