#pragma once

#include <raylib.h>

#include <cstdint>

namespace cosmic {

struct InputConfiguration {
    int32_t up_key = KEY_W;
    int32_t down_key = KEY_S;
    int32_t roll_left_key = KEY_A;
    int32_t roll_right_key = KEY_D;
};

struct Configuration {
    InputConfiguration input;
};

}  // namespace cosmic