#pragma once

#include <vector>
#include <kloss/block.hpp>

namespace kloss {
namespace creator {

class block_selection
{
public:
    using backup_type = std::vector<block>;

    bool const contains(block_ptr const& element) const;
    void insert(block_ptr const& element);
    void remove(block_ptr const& element);
    void clear();

    backup_type backup() const;
    void restore(backup_type const& backup);

    std::vector<block_ptr>::iterator begin();
    std::vector<block_ptr>::iterator end();

private:
    std::vector<block_ptr> container_;
};

} // namespace creator
} // namespace kloss
