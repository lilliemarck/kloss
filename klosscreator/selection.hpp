#pragma once

#include <vector>
#include <boost/optional.hpp>

namespace kloss {
namespace creator {

template <typename Policy>
class selection
{
public:
    void single_select(typename Policy::pick_type const& pick)
    {
        reference_.reset();

        if (Policy::has_selection(pick))
        {
            auto const& selection = Policy::get_selection(pick);

            if (!contains(selection_, selection))
            {
                selection_.clear();
                selection_.push_back(selection);
            }

            reference_ = Policy::get_intersection(pick);
        }
        else
        {
            selection_.clear();
        }
    }

    void multi_select(typename Policy::pick_type const& pick)
    {
        reference_.reset();

        if (Policy::has_selection(pick))
        {
            auto const& selection = Policy::get_selection(pick);

            if (!contains(selection_, selection))
            {
                selection_.push_back(selection);
                reference_ = Policy::get_intersection(pick);
            }
            else
            {
                remove(selection_, selection);
            }
        }
    }

    std::vector<typename Policy::backup_type> backup() const
    {
        std::vector<typename Policy::backup_type> backup;
        backup.reserve(selection_.size());

        for (auto const& element : selection_)
        {
            backup.push_back(Policy::backup(element));
        }

        return backup;
    }

    void restore(std::vector<typename Policy::backup_type> const& backup)
    {
        assert(backup.size() == selection_.size());

        auto out_iter = selection_.begin();

        for (auto const& element : backup)
        {
            Policy::restore(*out_iter++, element);
        }
    }

    /**
     * Return the position of where the most recent selection happened.
     */
    boost::optional<cml::vector3f> reference() const
    {
        return reference_;
    }

    typename std::vector<typename Policy::selection_type>::iterator begin()
    {
        return selection_.begin();
    }

    typename std::vector<typename Policy::selection_type>::iterator end()
    {
        return selection_.end();
    }

private:
    std::vector<typename Policy::selection_type> selection_;
    boost::optional<cml::vector3f> reference_;
};

} // namespace creator
} // namespace kloss
