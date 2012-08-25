#include "move_vertex_tool.hpp"
#include <QMouseEvent>
#include <kloss/algorithm.hpp>
#include <kloss/buffer.h>
#include <kloss/math.hpp>
#include <klosscreator/gl_widget.hpp>
#include <klosscreator/pick.h>

namespace kloss {
namespace creator {
namespace {

static float const snap_size = 1.0f / 8.0f;

} // namespace

move_vertex_tool::move_vertex_tool(gl_widget& parent)
    : parent_(parent)
    , selection_(CreateCornerSelection())
    , drag_(nullptr)
    , document_lock_(parent.document())
{
}

move_vertex_tool::~move_vertex_tool()
{
    DestroyBuffer(drag_);
    DestroyCornerSelection(selection_);
}

void move_vertex_tool::mouse_press_event(QMouseEvent const& event)
{
    if (event.button() == Qt::LeftButton)
    {
        auto pick = parent_.pick_vertex(event.x(), event.y());
        bool did_select;

        if (event.modifiers() & Qt::ControlModifier)
        {
            did_select = MultiPickCorner(selection_, pick.get_ptr());
        }
        else
        {
            did_select = SinglePickCorner(selection_, pick.get_ptr());
        }

        if (did_select)
        {
            reference_ = CornerRefPosition(pick.get_ptr());

            DestroyBuffer(drag_);
            drag_ = CreateBuffer();
            BackupCornerSelection(selection_, drag_);

            document_lock_.lock();
        }

        parent_.update();
    }
}

void move_vertex_tool::mouse_release_event(QMouseEvent const& event)
{
    if (event.button() == Qt::LeftButton)
    {
        document_lock_.unlock();
        DestroyBuffer(drag_);
        drag_ = nullptr;
    }
}

void move_vertex_tool::mouse_move_event(QMouseEvent const& event)
{
    if (drag_)
    {
        auto mouse_ray = parent_.mouse_ray(event.x(), event.y());
        Vec3 position;

        if (ConstrainRay(parent_.get_constrain_algorithm(), &mouse_ray, reference_.get_ptr(), &position))
        {
            Vec3 translation;
            Vec3Subtract(&translation, &position, reference_.get_ptr());

            translation.X = round(translation.X, snap_size);
            translation.Y = round(translation.Y, snap_size);
            translation.Z = round(translation.Z, snap_size);

            RestoreCornerSelection(selection_, drag_);

            CornerRef* cornerRefs = SelectedCorners(selection_);
            size_t count = SelectedCornerCount(selection_);

            for (size_t i = 0; i < count; ++i)
            {
                TranslateCornerRef(cornerRefs + i, &translation);
            }

            UpdateGroupVertexArray(parent_.group());
        }

        parent_.update();
    }
}

void move_vertex_tool::paint_gl()
{
    auto const& cursor_vertices = parent_.cursor_vertices();
    CornerRef const* cornerRefs = SelectedCorners(selection_);
    size_t count = SelectedCornerCount(selection_);

    for (size_t i = 0; i < count; ++i)
    {
        CornerRef const* corner = cornerRefs + i;

        if (corner->Flags & CORNER_REF_TOP)
        {
            draw(cursor_vertices, CornerTop(corner->Corner));
        }

        if (corner->Flags & CORNER_REF_BOTTOM)
        {
            draw(cursor_vertices, CornerBottom(corner->Corner));
        }
    }
}

} // namespace creator
} // namespace kloss
