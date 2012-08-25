#pragma once

#include <memory>
#include <vector>
#include <boost/optional.hpp>
#include <kloss/algorithm.hpp>
#include <kloss/block.hpp>
#include <kloss/corner_ref.hpp>
#include <kloss/group_instance.hpp>

namespace kloss {

struct pick
{
    block_ptr block;
    triangle triangle;
    Vec3 intersection;
};

struct viewport
{
    int x;
    int y;
    int width;
    int height;
};

class group
{
public:
    void insert(block_ptr const& block);

    template <typename Iterator>
    void insert(Iterator first, Iterator last)
    {
        for (auto iter = first; iter != last; ++iter)
        {
            insert(*iter);
        }
    }

    void remove(block_ptr const& block);

    template <typename Iterator>
    void remove(Iterator first, Iterator last)
    {
        for (auto iter = first; iter != last; ++iter)
        {
            remove(*iter);
        }

        update_vertex_array();
    }

    template <typename Function>
    void for_each_block(Function const& function) const
    {
        for (auto const& block : blocks_)
        {
            function(block);
        }
    }

    void insert(group_instance&& group_instance);

    template <typename Function>
    void for_each_group_instance(Function const& function) const
    {
        for (auto const& group_instance : group_instances_)
        {
            function(group_instance);
        }
    }

    template <typename Function>
    void for_each_group_instance(Function const& function)
    {
        for (auto& group_instance : group_instances_)
        {
            function(group_instance);
        }
    }

    pick const pick_block(ray const& ray) const;
    boost::optional<corner_ref> const pick_vertex(Mat4 const& model,
                                                  Mat4 const& projection,
                                                  viewport const& viewport,
                                                  Vec2 const& mouse) const;
    bounding_box bounding_box(Vec3 const& group_position) const;

    void draw() const;
    void update_vertex_array();

private:
    void append_vertices(block const& block);

    struct vertex
    {
        Vec3 normal;
        Vec3 position;
    };

    std::vector<block_ptr> blocks_;
    std::vector<group_instance> group_instances_;
    std::vector<vertex> vertices_;
};

void draw(group const& group);

} // namespace kloss
