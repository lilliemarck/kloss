#include "keypair.h"
#include <stdlib.h>

typedef struct Key Key;

struct Key
{
    bool Pressed;
    bool Selected;
};

struct KeyPair
{
    Key First;
    Key Second;
};

KeyPair *CreateKeyPair(void)
{
    return calloc(1, sizeof(KeyPair));
}

void DestroyKeyPair(KeyPair *pair)
{
    free(pair);
}

static void SetKeyState(Key *key, Key *other, bool pressed)
{
    if (key->Pressed != pressed)
    {
        key->Pressed = key->Selected = pressed;

        if (pressed)
        {
            other->Selected = false;
        }
        else
        {
            other->Selected = other->Pressed;
        }
    }
}

float KeyPairSetFirst(KeyPair *pair, bool pressed)
{
    SetKeyState(&pair->First, &pair->Second, pressed);
    return KeyPairValue(pair);
}

float KeyPairSetSecond(KeyPair *pair, bool pressed)
{
    SetKeyState(&pair->Second, &pair->First, pressed);
    return KeyPairValue(pair);
}

float KeyPairValue(KeyPair const *pair)
{
    if (pair->First.Selected)
    {
        return -1.0f;
    }
    else if (pair->Second.Selected)
    {
        return 1.0f;
    }
    else
    {
        return 0.0f;
    }
}
