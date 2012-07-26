#include "world.hpp"
#include <cassert>
#include <limits>
#include <kloss/math.hpp>
#include <GL/gl.h>

namespace kloss {

void world::insert(block_ptr const& block)
{
    assert(block);

    blocks_.push_back(block);
    append_vertices(*block);
}

pick world::pick(ray const& ray) const
{
    float nearest = std::numeric_limits<float>::max();
    block_ptr nearest_block;

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
                }
            }
        }
    }

    return {nearest_block, ray.origin + nearest * ray.direction};
}

boost::optional<cml::vector3f> world::pick_vertex(cml::matrix44f_c const& model,
                                                  cml::matrix44f_c const& projection,
                                                  viewport const& viewport,
                                                  cml::vector2f const& mouse) const
{
    float const radius = 5.0f;

    for (auto const& block : blocks_)
    {
        auto vertices = to_vertices(*block);

        for (cml::vector3f const& vertex : vertices)
        {
            auto screen_vertex = project(vertex, model, projection, viewport);

            if (screen_vertex)
            {
                (*screen_vertex)[1] = viewport.height - (*screen_vertex)[1];
                auto distance = kloss::distance(mouse, *screen_vertex);

                if (distance < radius)
                {
                    return vertex;
                }
            }
        }
    }

    return {};
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
