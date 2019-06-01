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
    const std::pair<size_t, size_t> HANGAR_OUT_LOWER;
    const std::pair<size_t, size_t> HANGAR_OUT_UPPER;
    const std::pair<size_t, size_t> LEFT_RUNWAY_END;
    const std::pair<size_t, size_t> RIGHT_RUNWAY_END;

    size_t PASSENGER_STOP;
    size_t UPPER_LANE_Y;
    size_t LOWER_LANE_Y;

    // ncurses color_pair constants
    const short WHITE = 1;
    const short RED   = 2;
    const short BLUE  = 3;
    const short GREEN = 4;
    const short PINK  = 5;

    // stats coordinates
    const short LEFT_RUNWAY_STAT  = 5;
    const short RIGHT_RUNWAY_STAT = 6;
    const short PA1_STAT          = 7;
    const short PA2_STAT          = 8;

    Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&)                 = default;
    Window& operator=(Window&&) = default;
    ~Window();

    void draw_foreground();
    
    size_t max_x() const { return win_->_maxx; };
    size_t max_y() const { return win_->_maxy; };

    void move_on_screen(const std::pair<unsigned, unsigned>& prev_coords,
                        const std::pair<unsigned, unsigned>& next_coords);
    void clear_pos(const std::pair<unsigned, unsigned>& coords);

    void free_pa(const std::pair<size_t, size_t> passenger_area);
    void occupy_pa(const std::pair<size_t, size_t> passenger_area);
    void free_runway(const std::pair<size_t, size_t> runway_start);
    void occupy_runway(const std::pair<size_t, size_t> runway_start);
    void break_runway(const std::pair<size_t, size_t> runway_start);
    void change_status(const short stat, std::string status, const short color);
    void update_loading_bar(const short, const int pegs);

   private:
    // fixed constants for drawing
    static const int RIGHT_PADDING {2};
    static const int RUNWAY_CONNECTION_WIDTH {6};
    static const int RUNWAY_WIDTH {7};
    static const int BOTTOM_PADDING {5};
    static const int TOP_PADDING {15};

    std::unique_ptr<WINDOW, void (*)(WINDOW*)> win_;
    std::mutex mtx_;

    void ncurses_rectangle(int y1, int x1, int y2, int x2);
    void init_pairs();

    
    void draw_title();
    void draw_stats();
    void draw_keyfuncs();

    void place_connections();
    void place_runways(int start1x, int start2x);
    void place_hangar();
    void place_upper_pa();
    void place_lower_pa();
    void draw_plane_count(int incoming, int outgoing);
};