#include "world.hpp"
#include <GL/gl.h>

namespace kloss {
namespace {

cml::vector3f top(corner const& corner)
{
    return {corner.x, corner.y, corner.top};
}

cml::vector3f bottom(corner const& corner)
{
    return {corner.x, corner.y, corner.bottom};
}

} // namespace

void world::insert(block const& block)
{
    // front
    cml::vector3f normal = {0.0f, -1.0f, 0.0f};
    vertices_.push_back({normal, top   (block[0])});
    vertices_.push_back({normal, bottom(block[0])});
    vertices_.push_back({normal, top   (block[1])});
    vertices_.push_back({normal, top   (block[1])});
    vertices_.push_back({normal, bottom(block[0])});
    vertices_.push_back({normal, bottom(block[1])});

    // right
    normal = {1.0f, 0.0f, 0.0f};
    vertices_.push_back({normal, top   (block[1])});
    vertices_.push_back({normal, bottom(block[1])});
    vertices_.push_back({normal, top   (block[2])});
    vertices_.push_back({normal, top   (block[2])});
    vertices_.push_back({normal, bottom(block[1])});
    vertices_.push_back({normal, bottom(block[2])});

    // back
    normal = {0.0f, 1.0f, 0.0f};
    vertices_.push_back({normal, top   (block[2])});
    vertices_.push_back({normal, bottom(block[2])});
    vertices_.push_back({normal, top   (block[3])});
    vertices_.push_back({normal, top   (block[3])});
    vertices_.push_back({normal, bottom(block[2])});
    vertices_.push_back({normal, bottom(block[3])});

    // left
    normal = {-1.0f, 0.0f, 0.0f};
    vertices_.push_back({normal, top   (block[3])});
    vertices_.push_back({normal, bottom(block[3])});
    vertices_.push_back({normal, top   (block[0])});
    vertices_.push_back({normal, top   (block[0])});
    vertices_.push_back({normal, bottom(block[3])});
    vertices_.push_back({normal, bottom(block[0])});

    // top
    normal = {0.0f, 0.0f, 1.0f};
    vertices_.push_back({normal, top(block[3])});
    vertices_.push_back({normal, top(block[0])});
    vertices_.push_back({normal, top(block[2])});
    vertices_.push_back({normal, top(block[2])});
    vertices_.push_back({normal, top(block[0])});
    vertices_.push_back({normal, top(block[1])});

    // bottom
    normal = {0.0f, 0.0f, -1.0f};
    vertices_.push_back({normal, bottom(block[3])});
    vertices_.push_back({normal, bottom(block[2])});
    vertices_.push_back({normal, bottom(block[0])});
    vertices_.push_back({normal, bottom(block[0])});
    vertices_.push_back({normal, bottom(block[2])});
    vertices_.push_back({normal, bottom(block[1])});
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
