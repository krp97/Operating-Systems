#pragma once

#include <ncurses.h>
#include <chrono>
#include <functional>
#include <mutex>
#include <atomic>
#include <thread>
#include "direction.hpp"

class Ball
{
   public:
    Ball() = delete;
    Ball(std::chrono::milliseconds cooldown_ms, WINDOW*);
    Ball& operator=(const Ball& rhs) = delete;
    Ball(const Ball& ball)           = delete;
    Ball& operator=(Ball&& rhs) noexcept = default;
    Ball(Ball&& ball) noexcept           = default;
    ~Ball();

    void idle_func();
    void request_stop();

   private:
    std::atomic<bool> stop_request_;
    std::chrono::milliseconds speed_;
    Direction direction_;
    WINDOW* window_;

    std::thread thread_;
    static std::mutex mtx_;
    std::pair<unsigned, unsigned> coords;

    void move(WINDOW*, Direction& direction);
    void update_coords();
    void update_direction();
};