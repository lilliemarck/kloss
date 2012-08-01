#pragma once

#include <boost/noncopyable.hpp>
#include <kloss/world.hpp>
#include <klosscreator/block_selection.hpp>

namespace kloss {
namespace creator {

class document : boost::noncopyable
{
public:
    document();
    ~document();

    void lock() noexcept;
    void unlock() noexcept;
    bool is_locked() const;

    void copy();
    void paste();

    world world;
    block_selection block_selection;

private:
    unsigned lock_count_;
    std::vector<block> copied_blocks_;
};

} // namespace creator
} // namespace kloss
