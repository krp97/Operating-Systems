#pragma once

#include <ncurses.h>
#include <atomic>
#include <chrono>
#include <condition_variable>
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
    void stop();
    void idle_func();

   private:
    std::chrono::milliseconds speed_;
    Direction direction_;
    Window& win_;
    std::pair<unsigned, unsigned> coords;

    std::thread thread_;
    std::atomic<bool> stop_request_ {false};

    static std::atomic<bool> freeze_flag_;
    static std::mutex mtx_;
    static std::condition_variable c_v_;

    void move();
    void update_coords();
    void update_direction();

    void freeze();
};