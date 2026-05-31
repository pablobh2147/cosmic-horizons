#include "Common.hpp"

#include <raylib.h>

#include <cstdlib>

namespace cosmic {

// Random

float RandomFloat(float min, float max) noexcept {
    return min + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (max - min);
}

int32_t RandomInt(int32_t min, int32_t max) noexcept {
    return min + std::rand() % (max - min + 1);
}

glm::vec3 RandomVector(float min, float max) noexcept {
    return glm::vec3 {
        RandomFloat(min, max),
        RandomFloat(min, max),
        RandomFloat(min, max),
    };
}

// Rendering

glm::vec3 ProjectToScreen(const glm::vec3& pos, const glm::mat4& mvp) noexcept {
    glm::vec4 projected_pos = mvp * glm::vec4(pos, 1.0F);

    float x = ((projected_pos.x / projected_pos.w) * 0.5F + 0.5F) * GetScreenWidth();
    float y = (1.0F - ((projected_pos.y / projected_pos.w) * 0.5F + 0.5F)) * GetScreenHeight();

    return glm::vec3(x, y, projected_pos.w);
}

void RenderTriangle(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, Color color) noexcept {
    if (p0.z <= 0.0F || p1.z <= 0.0F || p2.z <= 0.0F) return;

    DrawLine(static_cast<int>(p0.x), static_cast<int>(p0.y), static_cast<int>(p1.x), static_cast<int>(p1.y), color);
    DrawLine(static_cast<int>(p1.x), static_cast<int>(p1.y), static_cast<int>(p2.x), static_cast<int>(p2.y), color);
    DrawLine(static_cast<int>(p2.x), static_cast<int>(p2.y), static_cast<int>(p0.x), static_cast<int>(p0.y), color);
}

void RenderTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::mat4& mvp, Color color) noexcept {
    glm::vec3 p0 = ProjectToScreen(v0, mvp);
    glm::vec3 p1 = ProjectToScreen(v1, mvp);
    glm::vec3 p2 = ProjectToScreen(v2, mvp);

    RenderTriangle(p0, p1, p2, color);
}

void RenderPoligon(std::span<const glm::vec3> vertices, const glm::mat4& mvp, Color color) noexcept {
    if (vertices.size() < 3) {
        return;
    }

    glm::vec3 v0;
    glm::vec3 v1 = ProjectToScreen(vertices[0], mvp);
    glm::vec3 v2 = ProjectToScreen(vertices[1], mvp);

    for (size_t i = 2; i < vertices.size(); ++i) {
        v0 = v1;
        v1 = v2;
        v2 = ProjectToScreen(vertices[i], mvp);

        RenderTriangle(v0, v1, v2, color);
    }
}

}  // namespace cosmic
