#pragma once

#include <vector>

template <typename T>
void append(std::vector<T>& vector, std::initializer_list<T> list)
{
    vector.insert(end(vector), list);
}

template <typename T>
void remove(std::vector<T>& vector, T const& element)
{
    vector.erase(remove(begin(vector), end(vector), element), end(vector));
}

float round_to_increment(float x, float increment);
float square(float x);
