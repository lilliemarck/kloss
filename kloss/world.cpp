#include "world.hpp"
#include <GL/gl.h>

namespace kloss {

void world::insert(block const& block)
{
    size_t base = vertices_.size();

    // vertices
    vertices_.push_back({block[0].x, block[0].y, block[0].top});
    vertices_.push_back({block[0].x, block[0].y, block[0].bottom});
    vertices_.push_back({block[1].x, block[1].y, block[1].top});
    vertices_.push_back({block[1].x, block[1].y, block[1].bottom});
    vertices_.push_back({block[2].x, block[2].y, block[2].top});
    vertices_.push_back({block[2].x, block[2].y, block[2].bottom});
    vertices_.push_back({block[3].x, block[3].y, block[3].top});
    vertices_.push_back({block[3].x, block[3].y, block[3].bottom});

    // front
    indices_.push_back(base + 0);
    indices_.push_back(base + 1);
    indices_.push_back(base + 2);
    indices_.push_back(base + 2);
    indices_.push_back(base + 1);
    indices_.push_back(base + 3);

    // right
    indices_.push_back(base + 2);
    indices_.push_back(base + 3);
    indices_.push_back(base + 4);
    indices_.push_back(base + 4);
    indices_.push_back(base + 3);
    indices_.push_back(base + 5);

    // back
    indices_.push_back(base + 4);
    indices_.push_back(base + 5);
    indices_.push_back(base + 6);
    indices_.push_back(base + 6);
    indices_.push_back(base + 5);
    indices_.push_back(base + 7);

    // left
    indices_.push_back(base + 6);
    indices_.push_back(base + 7);
    indices_.push_back(base + 0);
    indices_.push_back(base + 0);
    indices_.push_back(base + 7);
    indices_.push_back(base + 1);

    // top
    indices_.push_back(base + 0);
    indices_.push_back(base + 2);
    indices_.push_back(base + 6);
    indices_.push_back(base + 6);
    indices_.push_back(base + 2);
    indices_.push_back(base + 4);

    // bottom
    indices_.push_back(base + 1);
    indices_.push_back(base + 7);
    indices_.push_back(base + 3);
    indices_.push_back(base + 3);
    indices_.push_back(base + 7);
    indices_.push_back(base + 5);
}

void world::draw()
{
    if (!vertices_.empty())
    {
        glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, sizeof(cml::vector3f), vertices_.front().data());
        glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, indices_.data());
        glPopClientAttrib();
    }
}

} // namespace kloss
