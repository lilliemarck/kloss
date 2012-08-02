#pragma once

#include <algorithm>

namespace kloss {

template <typename Container, typename T>
bool contains(Container const& container, T const& value)
{
    return std::find(begin(container), end(container), value) != end(container);
}

template <typename Container, typename T>
void remove(Container& container, T const& value)
{
    container.erase(std::remove(begin(container), end(container), value), end(container));
}

template <typename T>
void min(T& a, T const& b)
{
    if (b < a)
    {
        a = b;
    }
}

template <typename T>
void max(T& a, T const& b)
{
    if (b > a)
    {
        a = b;
    }
}

} // namespace kloss
