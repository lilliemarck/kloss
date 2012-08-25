#include "pick.h"

bool SinglePick(PickInterface *interface, void *data, void *pick)
{
    if (pick)
    {
        if (!interface->IsSelected(data, pick))
        {
            interface->DeselectAll(data);
            interface->Select(data, pick);
        }

        return true;
    }
    else
    {
        interface->DeselectAll(data);
    }

    return false;
}

bool MultiPick(PickInterface *interface, void *data, void *pick)
{
    if (pick)
    {
        if (!interface->IsSelected(data, pick))
        {
            interface->Select(data, pick);
            return true;
        }
        else
        {
            interface->Deselect(data, pick);
        }
    }

    return false;
}
