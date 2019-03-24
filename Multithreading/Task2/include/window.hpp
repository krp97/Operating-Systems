#pragma once

#include <ncurses.h>
#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>

class Window
{
   public:
    Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&)                 = default;
    Window& operator=(Window&&) = default;
    ~Window();

    void move_on_screen(std::pair<unsigned, unsigned>& prev_coords,
                        std::pair<unsigned, unsigned>& next_coords);

    int max_x() { return win_->_maxx; };
    int max_y() { return win_->_maxy; };

   private:
    std::unique_ptr<WINDOW, void (*)(WINDOW*)> win_;
    std::mutex mtx_;
};