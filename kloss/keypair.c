#include "keypair.h"
#include <stdlib.h>

typedef struct Key Key;

struct Key
{
    bool pressed;
    bool selected;
};

struct keypair
{
    Key first;
    Key second;
};

keypair *create_keypair(void)
{
    return calloc(1, sizeof(struct keypair));
}

void destroy_keypair(keypair *pair)
{
    free(pair);
}

static void SetKeyState(Key *key, Key *other, bool pressed)
{
    if (key->pressed != pressed)
    {
        key->pressed = key->selected = pressed;

        if (pressed)
        {
            other->selected = false;
        }
        else
        {
            other->selected = other->pressed;
        }
    }
}

float set_keypair_first(keypair *pair, bool pressed)
{
    SetKeyState(&pair->first, &pair->second, pressed);
    return keypair_value(pair);
}

float set_keypair_second(keypair *pair, bool pressed)
{
    SetKeyState(&pair->second, &pair->first, pressed);
    return keypair_value(pair);
}

float keypair_value(keypair const *pair)
{
    if (pair->first.selected)
    {
        return -1.0f;
    }
    else if (pair->second.selected)
    {
        return 1.0f;
    }
    else
    {
        return 0.0f;
    }
}
