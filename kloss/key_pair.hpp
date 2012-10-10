#pragma once

/**
 * Converts two mutually exclusive inputs to a single output value.
 */
class key_pair
{
public:
    key_pair();

    /**
     * Update the key state of the first key and return the new value.
     */
    float set_first(bool pressed);
    /**
     * Update the key state of the second key and return the new value.
     */
    float set_second(bool pressed);
    /**
     * Returns the value which is -1 if the first key is selected, 1 if the
     * second key is selected or 0 if none of the keys are pressed.
     */
    float value() const;

private:
    struct key
    {
        bool pressed;
        bool selected;
    };

    static void set_state(struct key& key, struct key& other, bool pressed);

    key first_;
    key second_;
};
