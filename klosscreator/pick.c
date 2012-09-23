#include "pick.h"

bool single_pick(pickprocs *interface, void *data, void *pick)
{
    if (pick)
    {
        if (!interface->is_selected(data, pick))
        {
            interface->deselect_all(data);
            interface->select(data, pick);
        }

        return true;
    }
    else
    {
        interface->deselect_all(data);
    }

    return false;
}

bool multi_pick(pickprocs *interface, void *data, void *pick)
{
    if (pick)
    {
        if (!interface->is_selected(data, pick))
        {
            interface->select(data, pick);
            return true;
        }
        else
        {
            interface->deselect(data, pick);
        }
    }

    return false;
}
