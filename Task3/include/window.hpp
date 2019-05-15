#pragma once
#include <memory>
#include <mutex>
#include <ncurses.h>

class Window {
public:
    Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = default;
    Window& operator=(Window&&) = default;
    ~Window();

    void draw_foreground();
    void place_connections();
    void place_runways(int start1x, int start2x);
    void place_hangars();

    int max_x() { return win_->_maxx; };
    int max_y() { return win_->_maxy; };

private:
    std::unique_ptr<WINDOW, void (*)(WINDOW*)> win_;
    std::mutex mtx_;
    const int runway_width = 9;

    void ncurses_rectangle(int y1, int x1, int y2, int x2);
};