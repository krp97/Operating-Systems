#include "../include/direction.hpp"
#include <iostream>
std::pair<short, short> Direction::vertical   = {1, -1};
std::pair<short, short> Direction::horizontal = {-1, 1};
std::pair<short, short> Direction::diagonal   = {-1, -1};

void Direction::reflect(const std::pair<short, short>& reflection)
{
    direction_.first *= reflection.first;
    direction_.second *= reflection.second;
}

Direction Direction::random_direction()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-1, 1);
    std::cout << dis(gen) << ", " << dis(gen) << std::endl;
    return Direction({dis(gen), dis(gen)});
}