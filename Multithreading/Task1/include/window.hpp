#pragma once

#include <ncurses.h>
#include <chrono>
#include <iostream>
#include <memory>
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
    bool shutdown_flag_;
    std::vector<std::unique_ptr<Ball>> balls;
    std::thread key_thread_;
    void stop_all();
};