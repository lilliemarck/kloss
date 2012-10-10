#pragma once

#include <kloss/block.hpp>
#include <memory>

using group_ptr = std::shared_ptr<class group>;

class group
{
public:
    group();
    group(group const& rhs);
    ~group();

    vec3 get_position() const;
    void set_position(vec3 const& position);
    void update_vertex_array();
    void draw() const;

    void insert(block_ptr* blocks, std::size_t count);
    void remove(block_ptr* blocks, std::size_t count);
    std::size_t block_count() const;
    std::vector<block_ptr>::iterator begin_blocks();
    std::vector<block_ptr>::iterator end_blocks();

    void insert(group_ptr const& group);
    void merge(group_ptr const& group);
    void remove(group_ptr const& group);
    std::size_t group_count() const;
    std::vector<group_ptr>::iterator begin_groups();
    std::vector<group_ptr>::iterator end_groups();

private:
    struct vertex
    {
        vec3 normal;
        vec3 position;
    };

    struct data
    {
        void append(std::vector<triangle> const& triangles);

        std::vector<block_ptr> blocks;
        std::vector<group_ptr> groups;
        std::vector<vertex> vertices;
    };

    void draw_recursive() const;

    vec3 position_;
    std::shared_ptr<data> data_;
};
