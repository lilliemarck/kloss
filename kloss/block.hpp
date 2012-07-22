#pragma once

#include <array>

namespace kloss {

struct corner
{
    float x;
    float y;
    float top;
    float bottom;
};

using block = std::array<corner,4>;

} // namespace kloss
