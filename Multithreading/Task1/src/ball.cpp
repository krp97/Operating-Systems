#include "../include/ball.hpp"

Ball::Ball(float speed, std::shared_ptr<WINDOW> window, Direction direction)
    : speed_ {speed}, window_ {window}, direction_ {direction}
{
}

void Ball::move() {}

void Ball::idle_func() {}