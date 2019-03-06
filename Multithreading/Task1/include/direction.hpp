#pragma once

#include <functional>

class Direction
{
   public:
    // Possible vector reflections when moving in a box.
    static std::pair<short, short> vertical;
    static std::pair<short, short> horizontal;
    static std::pair<short, short> diagonal;

    Direction() = delete;
    Direction(std::pair<short, short> direction) : direction_ {direction} {}
    Direction(const Direction&) = default;
    Direction& operator=(const Direction&) = default;
    Direction(Direction&&) noexcept        = default;
    Direction& operator=(Direction&&) noexcept = default;
    ~Direction()                               = default;

    void reflect(const std::pair<short, short>& reflection);

    short get_x() { return direction_.first; }
    short get_y() { return direction_.second; }

   private:
    std::pair<short, short> direction_;
};

/*
// If you hit the wall on the left or right -> horizontal, that includes the
following:
|/ -1 -1
|\ 1 -1
|----------------------------------
// If you hit the wall on the top or the bottom -> vertical
-------------------
    /   \
   1 1  1 -1
------------------------------------
// If you hit a corner then do both, so

if(crossed left || crossed right)
    reflect horizontally
if(crossed top || crossed bottom)
    reflect vertically

-> Store the temp before reflecting, so you can adjust for both reflections and
you good.
*/