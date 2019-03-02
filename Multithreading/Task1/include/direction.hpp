#include <functional>

class Direction
{
    using short_pair = std::pair<short, short>;

   public:
    // Possible vector reflections when moving in a box.
    short_pair vertical   = {1, -1};
    short_pair horizontal = {-1, 1};
    short_pair diagonal   = {-1, -1};

    Direction() = delete;
    Direction(short_pair direction) : direction_ {direction} {}
    Direction(const Direction&) = default;
    Direction& operator=(const Direction&) = default;
    Direction(Direction&&)                 = default;
    Direction& operator=(Direction&&) = default;
    ~Direction()                      = default;

    void reflect(const short_pair& reflection)
    {
        direction_.first *= reflection.first;
        direction_.second *= reflection.second;
    }

    short get_x() { return direction_.first; }
    short get_y() { return direction_.second; }

   private:
    short_pair direction_;
};