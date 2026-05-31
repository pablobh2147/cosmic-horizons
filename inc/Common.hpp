#pragma once

#include <raylib.h>

#include <glm/glm.hpp>

#include <cstdint>
#include <span>

namespace cosmic {

float RandomFloat(float min, float max) noexcept;
int32_t RandomInt(int32_t min, int32_t max) noexcept;
glm::vec3 RandomVector(float min, float max) noexcept;

glm::vec3 ProjectToScreen(const glm::vec3& pos, const glm::mat4& mvp) noexcept;
void RenderTriangle(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, Color color) noexcept;
void RenderTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::mat4& mvp, Color color) noexcept;
void RenderPoligon(std::span<const glm::vec3> vertices, const glm::mat4& mvp, Color color) noexcept;

}  // namespace cosmic