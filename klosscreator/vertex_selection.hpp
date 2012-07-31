#pragma once

#include <vector>
#include <kloss/block.hpp>

namespace kloss {
namespace creator {

class vertex_selection
{
public:
    using backup_type = std::vector<corner>;

    bool const contains(vertex_ref const& element) const;
    void insert(vertex_ref& element);
    void remove(vertex_ref const& element);
    void clear();

    backup_type backup() const;
    void restore(backup_type const& backup);

    enum flags
    {
        top_flag = 1,
        bottom_flag = 2
    };

    struct corner
    {
        void translate(cml::vector3f const& units);
        cml::vector3f const top_position() const;
        cml::vector3f const bottom_position() const;

        block_ptr block;
        kloss::corner* corner;
        uint8_t flags;
    };

    std::vector<corner>::iterator begin();
    std::vector<corner>::iterator end();

private:
    std::vector<corner> container_;
};

} // namespace creator
} // namespace kloss
