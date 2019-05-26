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
    const std::pair<size_t, size_t> LEFT_RUNWAY_END;
    const std::pair<size_t, size_t> RIGHT_RUNWAY_END;

    size_t PASSENGER_STOP;
    size_t UPPER_LANE_Y;
    size_t LOWER_LANE_Y;

    Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&)                 = default;
    Window& operator=(Window&&) = default;
    ~Window();

    size_t max_x() const { return win_->_maxx; };
    size_t max_y() const { return win_->_maxy; };

    void draw_foreground();
    void move_on_screen(const std::pair<unsigned, unsigned>& prev_coords,
                        const std::pair<unsigned, unsigned>& next_coords);
    void clear_pos(const std::pair<unsigned, unsigned>& coords);

   private:
    // ncurses color_pair constants
    const short WHITE = 1;
    const short RED   = 2;

    // fixed constants for drawing
    static const int RIGHT_PADDING {2};
    static const int RUNWAY_CONNECTION_WIDTH {6};
    static const int RUNWAY_WIDTH {7};
    static const int BOTTOM_PADDING {5};
    static const int TOP_PADDING {8};

    std::unique_ptr<WINDOW, void (*)(WINDOW*)> win_;
    std::mutex mtx_;

    void ncurses_rectangle(int y1, int x1, int y2, int x2);
    void init_pairs();

    // Draws the strip between the hangar and runways.
    void place_connections();
    void place_runways(int start1x, int start2x);
    void place_hangar();
    void place_passenger_areas();
};