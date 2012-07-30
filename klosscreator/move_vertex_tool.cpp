#include "move_vertex_tool.hpp"
#include <QMouseEvent>
#include <kloss/algorithm.hpp>
#include <kloss/math.hpp>
#include <klosscreator/gl_widget.hpp>

namespace kloss {
namespace creator {

move_vertex_tool::move_vertex_tool(gl_widget& parent) : parent_(parent)
{
}

void move_vertex_tool::mouse_press_event(QMouseEvent const& event)
{
    if (event.button() == Qt::LeftButton)
    {
        auto pick = parent_.pick_vertex(event.x(), event.y());

        if (event.modifiers() & Qt::ControlModifier)
        {
            multi_select(pick);
        }
        else
        {
            single_select(pick);
        }

        parent_.update();
    }
}

void move_vertex_tool::mouse_release_event(QMouseEvent const& event)
{
    if (event.button() == Qt::LeftButton)
    {
        drag_.reset();
    }
}

void move_vertex_tool::mouse_move_event(QMouseEvent const& event)
{
    if (drag_)
    {
        auto mouse_ray = parent_.mouse_ray(event.x(), event.y());
        auto position = constrain(parent_.get_constrain_algorithm(), mouse_ray, drag_->reference);

        if (position)
        {
            cml::vector3f translation = *position - drag_->reference;

            translation[0] = std::round(translation[0]);
            translation[1] = std::round(translation[1]);
            translation[2] = std::round(translation[2]);

            drag_->restore(selection_);

            for (auto& vertex : selection_)
            {
                vertex.translate(translation);
            }

            parent_.world().update_vertex_array();
        }

        parent_.update();
    }
}

void move_vertex_tool::paint_gl()
{
    auto const& cursor_vertices = parent_.cursor_vertices();

    for (auto const& vertex : selection_)
    {
        draw(cursor_vertices, vertex.to_vector());
    }
}

void move_vertex_tool::single_select(boost::optional<vertex_ref> const& pick)
{
    if (pick)
    {
        if (!contains(selection_, *pick))
        {
            selection_.clear();
            selection_.push_back(*pick);
        }

        drag_ = drag(pick->to_vector(), selection_);
    }
    else
    {
        selection_.clear();
    }
}

void move_vertex_tool::multi_select(boost::optional<vertex_ref> const& pick)
{
    if (pick)
    {
        if (!contains(selection_, *pick))
        {
            selection_.push_back(*pick);
            drag_ = drag(pick->to_vector(), selection_);
        }
        else
        {
            remove(selection_, *pick);
        }
    }
}

move_vertex_tool::drag::drag(cml::vector3f const& reference, std::vector<vertex_ref> const& vertices)
    : reference(reference)
{
    original_corners.reserve(vertices.size());

    for (auto const& vertex : vertices)
    {
        original_corners.push_back(vertex.corner());
    }
}

void move_vertex_tool::drag::restore(std::vector<vertex_ref>& vertices)
{
    assert(original_corners.size() == vertices.size());

    auto out_iter = begin(vertices);

    for (auto const& corner : original_corners)
    {
        (*out_iter++).corner() = corner;
    }
}

} // namespace creator
} // namespace kloss
