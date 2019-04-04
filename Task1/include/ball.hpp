#pragma once

#include <ncurses.h>
#include <atomic>
#include <chrono>
#include <functional>
#include <mutex>
#include <thread>
#include "direction.hpp"

class Ball
{
   public:
    Ball() = delete;
    Ball(std::chrono::milliseconds cooldown_ms, WINDOW*);
    Ball& operator=(const Ball& rhs) = delete;
    Ball(const Ball& ball)           = delete;
    Ball& operator=(Ball&& rhs) = default;
    Ball(Ball&& ball)           = default;
    ~Ball();

    void start();
    void idle_func();

   private:
    std::atomic_bool stop_request_;
    std::chrono::milliseconds speed_;
    Direction direction_;
    WINDOW* window_;

    std::thread thread_;
    static std::mutex mtx_;
    std::pair<unsigned, unsigned> coords;

    void move();
    void update_coords();
    void update_direction();
    void move_on_screen(std::pair<unsigned, unsigned>& prev_coords);
};