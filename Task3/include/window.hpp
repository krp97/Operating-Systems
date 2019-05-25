#pragma once
#include <ncurses.h>
#include <memory>
#include <mutex>

class Window
{
   public:
    // Terminal specific constants
    const std::pair<size_t, size_t> LEFT_RUNWAY_START;
    const std::pair<size_t, size_t> RIGHT_RUNWAY_START;
    const std::pair<size_t, size_t> HANGAR_OUT;
    const std::pair<size_t, size_t> UPPER_PASSENGER_AREA;
    const std::pair<size_t, size_t> LOWER_PASSENGER_AREA;

    Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&)                 = default;
    Window& operator=(Window&&) = default;
    ~Window();

    size_t max_x() const { return win_->_maxx; };
    size_t max_y() const { return win_->_maxy; };

    void draw_foreground();

   private:
    // ncurses color_pair constants
    const short WHITE = 1;
    const short RED   = 2;

    // fixed constants for drawing
    static const int RUNWAY_WIDTH {7};
    static const int BOTTOM_PADDING {2};
    static const int TOP_PADDING {8};
    
    std::unique_ptr<WINDOW, void (*)(WINDOW*)> win_;
    std::mutex mtx_;

    void ncurses_rectangle(int y1, int x1, int y2, int x2);
    void init_pairs();

    // Draws the strip between the hangar and runways.
    void place_connections();
    void place_runways(int start1x, int start2x);
    void place_hangar();
};