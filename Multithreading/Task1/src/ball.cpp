#include "../include/ball.hpp"
#include <iostream>

Ball::Ball(std::chrono::milliseconds speed, WINDOW* window, Direction direction)
    : speed_ {speed},
      window_ {window},
      direction_ {direction},
      stop_request_ {false}
{
    thread_ = std::thread([&]() { idle_func(speed, window, direction); });
}

void Ball::move(std::chrono::milliseconds speed, WINDOW* window,
                Direction& direction)
{  // TODO:
    /*
    Ball::mtx_.lock();
    std::cout << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(speed));
    Ball::mtx_.unlock();*/
}

void Ball::idle_func(std::chrono::milliseconds speed, WINDOW* window,
                     Direction& direction)
{
    while (true)  // fix
    {
        move(speed, window, direction);
        std::this_thread::sleep_for(std::chrono::milliseconds(speed));
    }
}

Ball::~Ball() { thread_.join(); }