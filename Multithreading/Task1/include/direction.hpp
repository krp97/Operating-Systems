#pragma once

#include <functional>
#include <random>

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

    static Direction random_direction();
    void reflect(const std::pair<short, short>& reflection);

    short get_x() { return direction_.first; }
    short get_y() { return direction_.second; }

   private:
    std::pair<short, short> direction_;
};