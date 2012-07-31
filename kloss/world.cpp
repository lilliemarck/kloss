#include "world.hpp"
#include <cassert>
#include <limits>
#include <kloss/math.hpp>
#include <GL/gl.h>

namespace kloss {
namespace {

boost::optional<cml::vector3f> project(cml::vector3f const& point,
                                       cml::matrix44f_c const& model,
                                       cml::matrix44f_c const& projection,
                                       viewport const& viewport)
{
    cml::vector4f point4 = {point[0], point[1], point[2], 1.0f};
    cml::vector4f out = projection * model * point4;

    if (out[2] < -out[3] || out[2] > out[3])
    {
        return {};
    }

    out[0] /= out[3];
    out[1] /= out[3];
    out[2] /= out[3];

    out[0] = out[0] * 0.5 + 0.5f;
    out[1] = out[1] * 0.5 + 0.5f;
    out[0] = out[0] * viewport.width + viewport.x;
    out[1] = out[1] * viewport.height + viewport.y;

    return boost::optional<cml::vector3f>({out[0], out[1], out[2]});
}

} // namespace

void world::insert(block_ptr const& block)
{
    assert(block);

    blocks_.push_back(block);
    append_vertices(*block);
}

pick const world::pick_block(ray const& ray) const
{
    float nearest = std::numeric_limits<float>::max();
    block_ptr nearest_block;
    triangle nearest_triangle;

    for (auto const& block : blocks_)
    {
        auto triangles = to_triangles(*block);

        for (triangle const& triangle : triangles)
        {
            if (auto temp = intersect(ray, triangle))
            {
                if (*temp < nearest)
                {
                    nearest = *temp;
                    nearest_block = block;
                    nearest_triangle = triangle;
                }
            }
        }
    }

    return {nearest_block, nearest_triangle, ray.origin + nearest * ray.direction};
}

boost::optional<corner_ref> const world::pick_vertex(cml::matrix44f_c const& model,
                                                     cml::matrix44f_c const& projection,
                                                     viewport const& viewport,
                                                     cml::vector2f const& mouse) const
{
    float const radius = 5.0f;
    float nearest_distance = std::numeric_limits<float>::max();
    boost::optional<corner_ref> nearest_corner_ref;

    auto check_vertex = [&](cml::vector3f const& position, corner_ref const& ref, uint8_t flag)
    {
        auto screen_position = project(position, model, projection, viewport);

        if (screen_position)
        {
            (*screen_position)[1] = viewport.height - (*screen_position)[1];
            auto distance = kloss::distance(mouse, to_vector2(*screen_position));

            if (distance < radius && (*screen_position)[2] < nearest_distance)
            {
                nearest_distance = (*screen_position)[2];
                nearest_corner_ref = kloss::corner_ref(ref, flag);
            }
        }
    };

    for (auto const& block : blocks_)
    {
        auto corner_refs = to_corner_refs(block);

        for (auto const& corner_ref : corner_refs)
        {
            check_vertex(top(corner_ref),    corner_ref, corner_ref::top_flag);
            check_vertex(bottom(corner_ref), corner_ref, corner_ref::bottom_flag);
        }
    }

    return nearest_corner_ref;
}

void world::draw()
{
    if (!vertices_.empty())
    {
        glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);

        setup_light();
        glEnable(GL_DEPTH_TEST);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

        glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, sizeof(vertex), vertices_.front().normal.data());
        glVertexPointer(3, GL_FLOAT, sizeof(vertex), vertices_.front().position.data());
        glDrawArrays(GL_TRIANGLES, 0, vertices_.size());
        glPopClientAttrib();

        glPopAttrib();
    }
}

void world::update_vertex_array()
{
    vertices_.clear();

    for (auto const& block : blocks_)
    {
        append_vertices(*block);
    }
}

void world::append_vertices(block const& block)
{
    auto triangles = to_triangles(block);

    for (auto const& triangle : triangles)
    {
        cml::vector3f normal = make_normal(triangle);
        vertices_.push_back({normal, triangle.a});
        vertices_.push_back({normal, triangle.b});
        vertices_.push_back({normal, triangle.c});
    }
}

void world::setup_light()
{
    cml::vector4f position = {-1.0f, -2.0f, 3.0f, 0.0f};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, position.data());
}

} // namespace kloss
