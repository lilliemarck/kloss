#pragma once

#include <array>
#include <cstdint>

struct block;
struct corner;
struct vec3;

class corner_ref
{
public:
    enum flags
    {
        top = 1,
        bottom = 2
    };

    corner_ref();
    corner_ref(corner& corner) noexcept;
    corner_ref(corner_ref const& ref, std::uint8_t flags) noexcept;

    operator bool() const noexcept;
    corner* operator->() noexcept;
    corner const* operator->() const noexcept;
    corner& operator*() noexcept;
    corner const& operator*() const noexcept;
    corner* get() noexcept;
    corner const* get() const noexcept;

    std::uint8_t flags() const noexcept;
    void add_flags(std::uint8_t flags) noexcept;
    void clear_flags(std::uint8_t flags) noexcept;

private:
    corner* corner_;
    std::uint8_t flags_;
};

std::array<corner_ref,4> make_corner_refs(block& block);
void translate(corner_ref& ref, vec3 const& units);

/**
 * Return the position of either the top or bottom vertex depending on which
 * flag is set. Should not be used when both flags are set.
 */
struct vec3 get_position(corner_ref const& ref);
