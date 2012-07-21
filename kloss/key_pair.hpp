#pragma once

namespace kloss {

/**
 * Convert two mutually exclusive inputs to a single output value.
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
        key();
        void update(key& other, bool is_pressed);
        bool pressed;
        bool selected;
    };

    key first_key_;
    key second_key_;
};

} // namespace kloss
