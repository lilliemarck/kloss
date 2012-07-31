#pragma once

#include <vector>
#include <kloss/block.hpp>
#include <kloss/corner_ref.hpp>

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

    std::vector<corner_ref>::iterator begin();
    std::vector<corner_ref>::iterator end();

private:
    std::vector<corner_ref> container_;
};

} // namespace creator
} // namespace kloss
