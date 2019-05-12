#pragma once
#include <ncurses.h>
#include <memory>
#include <mutex>

class Window
{
   public:
    Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&)                 = default;
    Window& operator=(Window&&) = default;
    ~Window();

    void draw_foreground();
    void place_runways();
    void place_hangars();

    int max_x() { return win_->_maxx; };
    int max_y() { return win_->_maxy; };

   private:
    std::unique_ptr<WINDOW, void (*)(WINDOW*)> win_;
    std::mutex mtx_;

    void ncurses_rectangle(int y1, int x1, int y2, int x2);
};