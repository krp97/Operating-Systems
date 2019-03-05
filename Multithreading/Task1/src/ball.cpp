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
    coords  = {getmaxx(stdscr) / 3, getmaxy(stdscr) / 2};
}

void Ball::move(WINDOW* window, Direction& direction)
{
    auto prev_coords = coords;
    update_coords();

    Ball::mtx_.lock();
    mvwprintw(window_, prev_coords.first, prev_coords.second, " ");
    mvwprintw(window_, coords.first, coords.second, "*");
    wrefresh(window_);
    Ball::mtx_.unlock();
}

void Ball::update_coords()
{
    // TODO: movement of a ball
    coords.first += direction_.get_x();
    coords.second += direction_.get_y();
}

void Ball::update_direction() {}

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