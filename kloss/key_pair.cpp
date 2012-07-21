#include "key_pair.hpp"

namespace kloss {

key_pair::key_pair()
{
}

float key_pair::set_first(bool pressed)
{
    first_key_.update(second_key_, pressed);
    return value();
}

float key_pair::set_second(bool pressed)
{
    second_key_.update(first_key_, pressed);
    return value();
}

float key_pair::value() const
{
    if (first_key_.selected)
    {
        return -1.0f;
    }
    else if (second_key_.selected)
    {
        return 1.0f;
    }
    else
    {
        return 0.0f;
    }
}

key_pair::key::key() : pressed(false), selected(false)
{
}

void key_pair::key::update(key& other, bool is_pressed)
{
    if (is_pressed != pressed)
    {
        pressed = selected = is_pressed;

        if (is_pressed)
        {
            other.selected = false;
        }
        else
        {
            other.selected = other.pressed;
        }
    }
}

} // namespace kloss
