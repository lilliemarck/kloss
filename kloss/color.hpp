#pragma once

namespace kloss {

template <typename T>
struct color3
{
    T const* data() const
    {
        return reinterpret_cast<T const*>(this);
    }

    T red, green, blue;
};

using color3ub = color3<std::uint8_t>;

} // namespace kloss
