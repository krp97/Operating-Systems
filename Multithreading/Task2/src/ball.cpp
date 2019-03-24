#include "../include/ball.hpp"
#include <iostream>

Ball::Ball(std::chrono::milliseconds speed, Window& win)
    : speed_ {speed},
      direction_ {Direction::random_direction()},
      stop_request_ {false},
      win_ {win}
{
    coords = {win_.max_x() / 3, win_.max_y() / 2};
}

void Ball::start()
{
    thread_ = std::thread([&]() { idle_func(); });
}

void Ball::idle_func()
{
    while (!stop_request_.load())
    {
        move();
        std::this_thread::sleep_for(speed_);
    }
}

void Ball::move()
{
    auto prev_coords = coords;
    update_direction();
    update_coords();
    win_.move_on_screen(prev_coords, coords);
}

void Ball::update_direction()
{
    if (coords.first >= win_.max_x() - 1 || coords.first <= 1)
        direction_.reflect(Direction::horizontal);
    if (coords.second >= win_.max_y() - 1 || coords.second <= 1)
        direction_.reflect(Direction::vertical);
}

void Ball::update_coords()
{
    coords.first += direction_.get_x();
    coords.second += direction_.get_y();
}

Ball::~Ball()
{
    stop_request_.store(true);
    thread_.join();
}