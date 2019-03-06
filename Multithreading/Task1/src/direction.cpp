#include "../include/direction.hpp"

std::pair<short, short> Direction::vertical   = {1, -1};
std::pair<short, short> Direction::horizontal = {-1, 1};
std::pair<short, short> Direction::diagonal   = {-1, -1};

void Direction::reflect(const std::pair<short, short>& reflection)
{
    direction_.first *= reflection.first;
    direction_.second *= reflection.second;
}