#include "../include/ball.hpp"
#include <iostream>

std::atomic<bool> Ball::is_frozen_   = false;
std::atomic<bool> Ball::should_wake_ = false;
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

void Ball::freeze()
{
    std::unique_lock<std::mutex> lk(mtx_);
    if (is_frozen_.load())
    {
        lk.unlock();
        switch_freeze();
    }
    else
    {
        is_frozen_.store(true);
        lk.unlock();
        freeze_first();
    }
}

void Ball::freeze_first()
{
    std::unique_lock<std::mutex> lk(mtx_);
    c_v_.wait(lk, [&]() { return should_wake_.load() 
                            || stop_request_.load(); });
    is_frozen_.store(false);
    should_wake_.store(false);
}

void Ball::switch_freeze()
{
    should_wake_.store(true);
    c_v_.notify_one();
    while (is_frozen_.load())
        ;
    std::unique_lock<std::mutex> lk(mtx_);
    is_frozen_.store(true);
    c_v_.wait(lk, [&]() { return should_wake_.load() 
                            || stop_request_.load(); });
    should_wake_.store(false);
    is_frozen_.store(false);
}

void Ball::update_coords()
{
    coords.first += direction_.get_x();
    coords.second += direction_.get_y();
}

void Ball::stop()
{
    stop_request_.store(true);
    c_v_.notify_one();
}

Ball::~Ball() { thread_.join(); }