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

    int max_x() const { return win_->_maxx; };
    int max_y() const { return win_->_maxy; };

    int first_runway_start();
    int second_runway_start();
    void draw_foreground();

   private:
    std::unique_ptr<WINDOW, void (*)(WINDOW*)> win_;
    std::mutex mtx_;

    // ncurses color_pair constants
    const short WHITE = 1;
    const short RED   = 2;

    // drawing
    const int runway_width        = 9;
    const int bottom_line_padding = 3;
    const int top_line_padding    = 7;
    const std::pair<int, int> first_runw_start_;
    const std::pair<int, int> second_runw_start_;
    const std::pair<int, int> first_hangar_out_;
    const std::pair<int, int> second_hangar_out_;

    void ncurses_rectangle(int y1, int x1, int y2, int x2);
    void init_pairs();

    void place_connections();
    void place_runways(int start1x, int start2x);
    void place_hangars();
};