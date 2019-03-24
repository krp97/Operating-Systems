#pragma once

#include <ncurses.h>
#include <atomic>
#include <chrono>
#include <functional>
#include <mutex>
#include <thread>
#include "direction.hpp"
#include "window.hpp"

class Ball
{
   public:
    Ball() = delete;
    Ball(std::chrono::milliseconds cooldown_ms, Window& win);
    Ball& operator=(const Ball& rhs) = delete;
    Ball(const Ball& ball)           = delete;
    Ball& operator=(Ball&& rhs) = default;
    Ball(Ball&& ball)           = default;
    ~Ball();

    void start();
    void idle_func();

   private:
    std::atomic<bool> stop_request_;
    std::chrono::milliseconds speed_;
    Direction direction_;
    Window& win_;

    std::thread thread_;
    std::pair<unsigned, unsigned> coords;

    void move();
    void update_coords();
    void update_direction();
};