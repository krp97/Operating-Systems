#include "../include/ball.hpp"
#include <iostream>

std::mutex Ball::mtx_ = std::mutex();

Ball::Ball(std::chrono::milliseconds speed, WINDOW* window, Direction direction)
    : speed_ {speed},
      window_ {window},
      direction_ {direction},
      stop_request_ {false}
{
    thread_ = std::thread([&]() { idle_func(); });
    coords  = {window_->_maxx / 3, window_->_maxy / 2};
}

void Ball::move(WINDOW* window, Direction& direction)
{
    auto prev_coords = coords;
    update_direction();
    update_coords();

    Ball::mtx_.lock();
    mvwprintw(window_, prev_coords.second, prev_coords.first, " ");
    mvwprintw(window_, coords.second, coords.first, "*");
    wrefresh(window_);
    Ball::mtx_.unlock();
}

void Ball::update_coords()
{
    coords.first += direction_.get_x();
    coords.second += direction_.get_y();
}

void Ball::update_direction()
{
    if (coords.first >= window_->_maxx - 1 || coords.first <= 1)
        direction_.reflect(Direction::horizontal);
    if (coords.second >= window_->_maxy - 1 || coords.second <= 1)
        direction_.reflect(Direction::vertical);
}

void Ball::request_stop() { stop_request_ = true; }

void Ball::idle_func()
{
    while (!stop_request_)
    {
        move(window_, direction_);
        std::this_thread::sleep_for(speed_);
    }
}

Ball::~Ball() { thread_.join(); }