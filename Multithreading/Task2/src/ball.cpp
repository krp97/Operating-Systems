#include "../include/ball.hpp"
#include <iostream>

std::atomic<bool> Ball::freeze_flag_ = true;
std::mutex Ball::mtx_                = std::mutex();
std::condition_variable Ball::c_v_   = std::condition_variable();

Ball::Ball(std::chrono::milliseconds speed, Window& win)
    : speed_ {speed}, direction_ {Direction::random_direction()}, win_ {win}
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
    if (coords.second <= 1)
    {
        freeze();
        direction_.reflect(Direction::vertical);
    }
    else if (coords.second >= win_.max_y() - 1)
        direction_.reflect(Direction::vertical);
    if (coords.first >= win_.max_x() - 1 || coords.first <= 1)
        direction_.reflect(Direction::horizontal);
}

void Ball::update_coords()
{
    coords.first += direction_.get_x();
    coords.second += direction_.get_y();
}

void Ball::freeze()
{
    std::unique_lock<std::mutex> lk(mtx_);
    if (!freeze_flag_.load())
    {
        freeze_flag_.store(true);
        lk.unlock();
        c_v_.notify_one();
    }
    else
    {
        freeze_flag_.store(false);
        c_v_.wait(lk,
                  [this]() { return freeze_flag_.load() || stop_request_; });
    }
}

void Ball::stop()
{
    stop_request_.store(true);
    c_v_.notify_one();
}

Ball::~Ball() { thread_.join(); }