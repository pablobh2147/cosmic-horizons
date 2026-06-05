#pragma once

#include <cstdint>

namespace cosmic {

struct Statistics {
    int32_t asteroids_destroyed = 0;
    int32_t score = 0;
    int32_t level = 1;

    int32_t shots_fired = 0;
    int32_t misses = 0;

    float GetAccuracy() const noexcept {
        if (shots_fired == 0) {
            return 0.0F;
        }
        return 1.0F - static_cast<float>(misses) / static_cast<float>(shots_fired);
    }
};

}  // namespace cosmic
