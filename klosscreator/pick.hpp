#pragma once

class selector
{
public:
    virtual bool is_selected() = 0;
    virtual void select() = 0;
    virtual void deselect() = 0;
    virtual void deselect_all() = 0;
};

bool single_pick(selector& selector, bool pick);
bool multi_pick(selector& selector, bool pick);
