#include "group.hpp"
#include <cassert>
#include <limits>
#include <kloss/bounding_box.hpp>
#include <kloss/group_instance.hpp>
#include <kloss/math.hpp>
#include <GL/gl.h>

namespace kloss {
namespace {

boost::optional<Vec3> project(Vec3 const& point,
                               Mat4 const& model,
                               Mat4 const& projection,
                               viewport const& viewport)
{
    Mat4 transform;
    Vec3 out;

    Mat4Transform(&transform, &model, &projection);
    Vec3Transform(&out, &point, &transform);

    float w = transform.X.W * point.X +
              transform.Y.W * point.Y +
              transform.Z.W * point.Z +
              transform.T.W;

    if (out.Z < -w || out.Z > w)
    {
        return {};
    }

    out.X /= w;
    out.Y /= w;
    out.Z /= w;

    out.X = out.X * 0.5f + 0.5f;
    out.Y = out.Y * 0.5f + 0.5f;
    out.X = out.X * viewport.width + viewport.x;
    out.Y = out.Y * viewport.height + viewport.y;

    return boost::optional<Vec3>({out.X, out.Y, out.Z});
}

} // namespace

void group::insert(block_ptr const& block)
{
    assert(block);

    blocks_.push_back(block);
    append_vertices(*block);
}

void group::remove(block_ptr const& block)
{
    kloss::remove(blocks_, block);
}

void group::insert(group_instance&& group_instance)
{
    group_instances_.push_back(std::move(group_instance));
}

pick const group::pick_block(ray const& ray) const
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
                /*
                 * Use <= for distance comparsion here so that when there are
                 * multiple candidates for selection the last block added will
                 * be selected. It is useful after pasting that the new blocks
                 * can be dragged to a new location.
                 */
                if (*temp <= nearest)
                {
                    nearest = *temp;
                    nearest_block = block;
                    nearest_triangle = triangle;
                }
            }
        }
    }

    Vec3 intersection;
    Vec3AddScaled(&intersection, &ray.origin, &ray.direction, nearest);
    return {nearest_block, nearest_triangle, intersection};
}

boost::optional<corner_ref> const group::pick_vertex(Mat4 const& model,
                                                     Mat4 const& projection,
                                                     viewport const& viewport,
                                                     Vec2 const& mouse) const
{
    float const radius = 5.0f;
    float nearest_distance = std::numeric_limits<float>::max();
    boost::optional<corner_ref> nearest_corner_ref;

    auto check_vertex = [&](Vec3 const& position, corner_ref const& ref, uint8_t flag)
    {
        auto screen_position = project(position, model, projection, viewport);

        if (screen_position)
        {
            Vec2 screen_position_xy = {screen_position->X, viewport.height - screen_position->Y};

            auto distance = Vec2SquaredDistance(&mouse, &screen_position_xy);

            if (distance < radius && screen_position->Z < nearest_distance)
            {
                nearest_distance = screen_position->Z;
                nearest_corner_ref = kloss::corner_ref(ref, flag);
            }
        }
    };

    for (auto const& block : blocks_)
    {
        auto corner_refs = to_corner_refs(block);

        for (auto const& corner_ref : corner_refs)
        {
            check_vertex(top(*corner_ref),    corner_ref, corner_ref::top_flag);
            check_vertex(bottom(*corner_ref), corner_ref, corner_ref::bottom_flag);
        }
    }

    return nearest_corner_ref;
}

namespace {

void expand(bounding_box& bbox, corner const& corner)
{
    min(bbox.lower.X, corner.x);
    max(bbox.upper.X, corner.x);
    min(bbox.lower.Y, corner.y);
    max(bbox.upper.Y, corner.y);
    min(bbox.lower.Z, corner.bottom);
    max(bbox.upper.Z, corner.top);
}

} // namespace

bounding_box group::bounding_box(Vec3 const& group_position) const
{
    kloss::bounding_box bbox;

    if (!blocks_.empty())
    {
        for (auto const& block : blocks_)
        {
            for (auto corner : *block)
            {
                expand(bbox, corner);
            }
        }

        // Don't translate if there are no blocks
        translate(bbox, group_position);
    }

    for (auto const& group_instance : group_instances_)
    {
        expand(bbox, group_instance.bounding_box(group_position));
    }

    return bbox;
}

void group::draw() const
{
    if (!vertices_.empty())
    {
        glNormalPointer(GL_FLOAT, sizeof(vertex), &vertices_.front().normal);
        glVertexPointer(3, GL_FLOAT, sizeof(vertex), &vertices_.front().position);
        glDrawArrays(GL_TRIANGLES, 0, vertices_.size());
    }

    for (auto const& group_instance : group_instances_)
    {
        group_instance.draw();
    }
}

void group::update_vertex_array()
{
    vertices_.clear();

    for (auto const& block : blocks_)
    {
        append_vertices(*block);
    }
}

void group::append_vertices(block const& block)
{
    auto triangles = to_triangles(block);

    for (auto const& triangle : triangles)
    {
        Vec3 normal = make_normal(triangle);
        vertices_.push_back({normal, triangle.a});
        vertices_.push_back({normal, triangle.b});
        vertices_.push_back({normal, triangle.c});
    }
}

void draw(group const& group)
{
    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    Vec4 light_position = {-1.0f, -2.0f, 3.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, &light_position.X);

    glEnable(GL_DEPTH_TEST);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    group.draw();

    glPopClientAttrib();
    glPopAttrib();
}

} // namespace kloss
