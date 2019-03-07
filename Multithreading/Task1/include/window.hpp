#pragma once

#include <ncurses.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <atomic>
#include <thread>
#include <vector>
#include "ball.hpp"

class Window
{
   public:
    Window();
    Window(int h_lines, int v_lines, int x_start, int y_start);
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    Window& operator=(Window&&);
    Window(Window&&);
    ~Window();

    void release_the_hounds();
    

   private:
    WINDOW* window_;
    std::atomic<bool> shutdown_flag_;
    std::vector<std::unique_ptr<Ball>> balls;
    std::thread key_watcher_;

    void stop_all();
    void pressed_exit();
    void wait_n_check_shutdwn(std::chrono::milliseconds);
};