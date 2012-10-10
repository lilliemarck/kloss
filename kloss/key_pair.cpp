#include "key_pair.hpp"

key_pair::key_pair() : first_(), second_()
{
}

float key_pair::set_first(bool pressed)
{
    set_state(first_, second_, pressed);
    return value();
}

float key_pair::set_second(bool pressed)
{
    set_state(second_, first_, pressed);
    return value();
}

float key_pair::value() const
{
    if (first_.selected)
    {
        return -1.0f;
    }
    else if (second_.selected)
    {
        return 1.0f;
    }
    else
    {
        return 0.0f;
    }
}

void key_pair::set_state(struct key& key, struct key& other, bool pressed)
{
    if (key.pressed != pressed)
    {
        key.pressed = key.selected = pressed;

        if (pressed)
        {
            other.selected = false;
        }
        else
        {
            other.selected = other.pressed;
        }
    }
}
