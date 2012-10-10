#include "pick.hpp"

bool single_pick(selector& selector, bool pick)
{
    if (pick)
    {
        if (!selector.is_selected())
        {
            selector.deselect_all();
            selector.select();
        }

        return true;
    }
    else
    {
        selector.deselect_all();
    }

    return false;
}

bool multi_pick(selector& selector, bool pick)
{
    if (pick)
    {
        if (!selector.is_selected())
        {
            selector.select();
            return true;
        }
        else
        {
            selector.deselect();
        }
    }

    return false;
}
