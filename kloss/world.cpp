#include "world.hpp"
#include <cassert>
#include <limits>
#include <GL/gl.h>

namespace kloss {

void world::insert(block_ptr const& block)
{
    assert(block);

    blocks_.push_back(block);

    auto triangles = to_triangles(*block);

    for (auto const& triangle : triangles)
    {
        cml::vector3f normal = make_normal(triangle);
        vertices_.push_back({normal, triangle.a});
        vertices_.push_back({normal, triangle.b});
        vertices_.push_back({normal, triangle.c});
    }
}

block_ptr world::pick(ray const& ray) const
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

    return nearest_block;
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

void world::setup_light()
{
    cml::vector4f position = {-1.0f, -2.0f, 3.0f, 0.0f};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, position.data());
}

} // namespace kloss
