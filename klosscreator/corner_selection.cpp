#include "corner_selection.hpp"
#include <kloss/block.hpp>
#include <kloss/corner_ref.hpp>
#include <klosscreator/pick.hpp>
#include <cassert>

std::size_t corner_selection::corner_count()
{
    return refs_.size();
}

corner_ref* corner_selection::get_corners()
{
    return refs_.data();
}

std::vector<corner> corner_selection::backup() const
{
    std::vector<corner> corners;

    for (auto const& ref : refs_)
    {
        corners.push_back(*ref);
    }

    return corners;
}

void corner_selection::restore(std::vector<corner> const& backup)
{
    assert(backup.size() == refs_.size());

    for (std::size_t i = 0; i < refs_.size(); ++i)
    {
        *refs_[i] = backup[i];
    }
}

class corner_selector : public selector
{
public:
    corner_selector(corner_selection& selection, corner_ref const& ref)
        : selection_(selection)
        , ref_(ref)
    {}

    bool is_selected() override
    {
        for (auto const& ref : selection_.refs_)
        {
            if (ref.get() == ref_.get() && ref.flags() & ref_.flags())
            {
                return true;
            }
        }

        return false;
    }

    void select() override
    {
        for (auto& ref : selection_.refs_)
        {
            if (ref.get() == ref_.get())
            {
                ref.add_flags(ref.flags());
                return;
            }
        }

        selection_.refs_.push_back(ref_);
    }

    void deselect() override
    {
        for (auto it = begin(selection_.refs_); it != end(selection_.refs_); ++it)
        {
            if (it->get() == ref_.get())
            {
                it->clear_flags(ref_.flags());

                if (it->flags() == 0)
                {
                    selection_.refs_.erase(it);
                }

                return;
            }
        }
    }

    void deselect_all() override
    {
        selection_.refs_.clear();
    }

private:
    corner_selection& selection_;
    corner_ref const& ref_;
};

bool single_pick(corner_selection& selection, corner_ref const* ref)
{
    corner_selector selector(selection, *ref);
    return single_pick(selector, ref);
}

bool multi_pick(corner_selection& selection, corner_ref const* ref)
{
    corner_selector selector(selection, *ref);
    return multi_pick(selector, ref);
}
