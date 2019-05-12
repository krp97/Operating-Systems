#include "../include/ball.hpp"
#include <iostream>

std::mutex Ball::mtx_;

Ball::Ball(std::chrono::milliseconds speed, WINDOW* window)
    : speed_ {speed},
      window_ {window},
      direction_ {Direction::random_direction()},
      stop_request_ {false}
{
    coords = {window_->_maxx / 3, window_->_maxy / 2};
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
    move_on_screen(prev_coords);
}

void Ball::update_direction()
{
    if (coords.first >= window_->_maxx - 1 || coords.first <= 1)
        direction_.reflect(Direction::horizontal);
    if (coords.second >= window_->_maxy - 1 || coords.second <= 1)
        direction_.reflect(Direction::vertical);
}

void Ball::update_coords()
{
    coords.first += direction_.get_x();
    coords.second += direction_.get_y();
}

void Ball::move_on_screen(std::pair<unsigned, unsigned>& prev_coords)
{
    std::lock_guard<std::mutex> l_g(Ball::mtx_);
    mvwprintw(window_, prev_coords.second, prev_coords.first, " ");
    mvwprintw(window_, coords.second, coords.first, "o");
    box(window_, 0, 0);
    wrefresh(window_);
}

Ball::~Ball()
{
    stop_request_.store(true);
    thread_.join();
}