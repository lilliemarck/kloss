#include "group.hpp"
#include <kloss/algorithm.hpp>
#include <kloss/memory.hpp>
#include <GL/gl.h>

group::group()
    : position_()
    , data_(std::make_shared<data>())
{
}

group::group(group const& rhs)
    : position_(rhs.position_)
    , data_(rhs.data_)
{
}

group::~group()
{
}

vec3 group::get_position() const
{
    return position_;
}

void group::set_position(vec3 const& position)
{
    position_ = position;
}

void group::update_vertex_array()
{
    data_->vertices.clear();
    std::vector<triangle> triangles;

    for (auto& block : data_->blocks)
    {
        get_triangles(*block, triangles);
        data_->append(triangles);
        triangles.clear();
    }
}

void group::draw() const
{
    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    vec4 light = {-1.0f, -2.0f, 3.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, &light.x);

    glEnable(GL_DEPTH_TEST);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    draw_recursive();

    glPopClientAttrib();
    glPopAttrib();
}

void group::insert(block_ptr* blocks, std::size_t count)
{
    std::vector<triangle> triangles;

    for (std::size_t i = 0; i < count; ++i)
    {
        block_ptr& block = blocks[i];
        data_->blocks.push_back(block);
        get_triangles(*block, triangles);
        data_->append(triangles);
        triangles.clear();
    }
}

void group::remove(block_ptr* blocks, std::size_t count)
{
    for (std::size_t i = 0; i < count; ++i)
    {
        ::remove(data_->blocks, blocks[i]);
    }

    update_vertex_array();
}

std::size_t group::block_count() const
{
    return data_->blocks.size();
}

std::vector<block_ptr>::iterator group::begin_blocks()
{
    return begin(data_->blocks);
}

std::vector<block_ptr>::iterator group::end_blocks()
{
    return end(data_->blocks);
}

void group::insert(group_ptr const& group)
{
    data_->groups.push_back(group);
}

void group::merge(group_ptr const& group)
{
    for (auto& block : group->data_->blocks)
    {
        block_ptr copy = std::make_shared<struct block>(*block);
        translate(*copy, group->position_);
        data_->blocks.push_back(copy);
    }

    for (auto& child : group->data_->groups)
    {
        group_ptr copy = make_unique<class group>(*child);
        copy->position_ += group->position_;
        insert(std::move(copy));
    }

    update_vertex_array();
}

void group::remove(group_ptr const& group)
{
    ::remove(data_->groups, group);
}

std::size_t group::group_count() const
{
    return data_->groups.size();
}

std::vector<group_ptr>::iterator group::begin_groups()
{
    return begin(data_->groups);
}

std::vector<group_ptr>::iterator group::end_groups()
{
    return end(data_->groups);
}

void group::data::append(std::vector<triangle> const& triangles)
{
    for (auto const& triangle : triangles)
    {
        vec3 normal = norm(make_normal(triangle));

        ::append(vertices,
        {
            {normal, triangle.a},
            {normal, triangle.b},
            {normal, triangle.c}
        });
    }
}

void group::draw_recursive() const
{
    vertex const* vertices = data_->vertices.data();

    glPushMatrix();
    glTranslatef(position_.x, position_.y, position_.z);
    glNormalPointer(GL_FLOAT, sizeof(struct vertex), &vertices->normal);
    glVertexPointer(3, GL_FLOAT, sizeof(struct vertex), &vertices->position);
    glDrawArrays(GL_TRIANGLES, 0, data_->vertices.size());

    for (auto& group : data_->groups)
    {
        group->draw_recursive();
    }

    glPopMatrix();
}
