#include "../include/window.hpp"

// Runway start point is a horizontal half point, shited to the left or right
// by: sum(half the runway width, half the middle connector width).

Window::Window()
    : LEFT_RUNWAY_START {getmaxx(stdscr) -
                             (2 * RUNWAY_WIDTH + RUNWAY_CONNECTION_WIDTH +
                              RIGHT_PADDING) +
                             RUNWAY_WIDTH / 2,
                         getmaxy(stdscr) - BOTTOM_PADDING - 4},
      RIGHT_RUNWAY_START {
          LEFT_RUNWAY_START.first + RUNWAY_WIDTH + RUNWAY_CONNECTION_WIDTH,
          getmaxy(stdscr) - BOTTOM_PADDING - 4},
      HANGAR_OUT {16, getmaxy(stdscr) - (BOTTOM_PADDING + 2)},
      PASSENGER_STOP {static_cast<size_t>(getmaxx(stdscr) / 2)},
      UPPER_LANE_Y {
          static_cast<size_t>(getmaxy(stdscr) - (BOTTOM_PADDING + 4))},
      LOWER_LANE_Y {
          static_cast<size_t>(getmaxy(stdscr) - (BOTTOM_PADDING + 2))},
      LEFT_RUNWAY_END {LEFT_RUNWAY_START.first, 8},
      RIGHT_RUNWAY_END {RIGHT_RUNWAY_START.first, 8},
      win_(newwin(getmaxy(stdscr), getmaxx(stdscr), 0, 0), [](WINDOW* w) {
          delwin(w);
          endwin();
      })

{
    use_default_colors();
    draw_foreground();
    curs_set(false);
}

void Window::init_pairs()
{
    init_pair(WHITE, COLOR_WHITE, -1);
    init_pair(RED, COLOR_RED, -1);
    init_pair(BLUE, COLOR_BLUE, -1);
}

void Window::draw_foreground()
{
    init_pairs();
    std::lock_guard<std::mutex> l_g(mtx_);
    box(win_.get(), 0, 0);
    place_connections();
    place_hangar();
    place_upper_pa();
    place_lower_pa();
    wrefresh(win_.get());
}

void Window::place_connections()
{
    wattron(win_.get(), COLOR_PAIR(WHITE));
    constexpr int x_start = 17;
    const int x_end       = max_x() - 2;
    const int line_len    = x_end - 17;
    mvwhline(win_.get(), max_y() - BOTTOM_PADDING, x_start, ACS_HLINE,
             line_len);

    // Middle
    const int half_len    = x_end - RUNWAY_WIDTH - 2;
    const int mid_start   = half_len - 3;
    constexpr int mid_len = 6;
    mvwhline(win_.get(), max_y() - BOTTOM_PADDING - 4, mid_start, ACS_HLINE,
             mid_len);

    // Left
    const int left_len = mid_start - RUNWAY_WIDTH - x_start;
    mvwhline(win_.get(), max_y() - BOTTOM_PADDING - 4, x_start, ACS_HLINE,
             left_len);
    wattroff(win_.get(), COLOR_PAIR(WHITE));
    place_runways(x_start + left_len, mid_start + mid_len);
}

void Window::place_runways(int startx1, int startx2)
{
    constexpr int runway_start = TOP_PADDING;
    const int runway_end       = max_y() - BOTTOM_PADDING - 4;
    const int runway_len       = runway_end - runway_start;
    wattron(win_.get(), COLOR_PAIR(WHITE));

    // Left runway
    mvwvline(win_.get(), runway_start, startx1, ACS_VLINE, runway_len);
    mvwvline(win_.get(), runway_start, startx1 + RUNWAY_WIDTH - 1, ACS_VLINE,
             runway_len);
    mvwaddch(win_.get(), runway_end, startx1, ACS_LRCORNER);
    mvwaddch(win_.get(), runway_end, startx1 + RUNWAY_WIDTH - 1, ACS_LLCORNER);

    // Right runway
    mvwvline(win_.get(), runway_start, startx2, ACS_VLINE, runway_len);
    mvwvline(win_.get(), runway_start, startx2 + RUNWAY_WIDTH - 1, ACS_VLINE,
             runway_len + 4);
    mvwaddch(win_.get(), runway_end, startx2, ACS_LRCORNER);
    mvwaddch(win_.get(), max_y() - BOTTOM_PADDING, max_x() - 2, ACS_LRCORNER);
    wattroff(win_.get(), COLOR_PAIR(WHITE));
}

void Window::place_hangar()
{
    wattron(win_.get(), COLOR_PAIR(RED));
    ncurses_rectangle(max_y() - 11, 1, max_y() - 3, 15);
    mvwprintw(win_.get(), max_y() - 7, 5, "Hangar");
    wattroff(win_.get(), COLOR_PAIR(RED));
}

void Window::place_upper_pa()
{
    size_t area_width   = 6;
    size_t area_height  = 2;
    size_t x_start      = (max_x() / 2) - (area_width / 2);
    size_t upper_area_y = max_y() - BOTTOM_PADDING - 4 - area_height - 1;
    ncurses_rectangle(upper_area_y, x_start, upper_area_y + area_height,
                      x_start + area_width);
    mvwprintw(win_.get(), upper_area_y + 1, x_start + 2, "PA1");
}

void Window::place_lower_pa()
{
    size_t area_width   = 6;
    size_t area_height  = 2;
    size_t x_start      = (max_x() / 2) - (area_width / 2);
    size_t lower_area_y = max_y() - BOTTOM_PADDING + 1;
    ncurses_rectangle(lower_area_y, x_start, lower_area_y + area_height,
                      x_start + area_width);

    mvwprintw(win_.get(), lower_area_y + 1, x_start + 2, "PA2");
}

void Window::ncurses_rectangle(int y1, int x1, int y2, int x2)
{
    mvwhline(win_.get(), y1, x1, 0, x2 - x1);
    mvwhline(win_.get(), y2, x1, 0, x2 - x1);
    mvwvline(win_.get(), y1, x1, 0, y2 - y1);
    mvwvline(win_.get(), y1, x2, 0, y2 - y1);
    mvwaddch(win_.get(), y1, x1, ACS_ULCORNER);
    mvwaddch(win_.get(), y2, x1, ACS_LLCORNER);
    mvwaddch(win_.get(), y1, x2, ACS_URCORNER);
    mvwaddch(win_.get(), y2, x2, ACS_LRCORNER);
}

void Window::move_on_screen(const std::pair<unsigned, unsigned>& prev_coords,
                            const std::pair<unsigned, unsigned>& next_coords)
{
    std::lock_guard<std::mutex> l_g(mtx_);
    mvwprintw(win_.get(), prev_coords.second, prev_coords.first, " ");
    mvwprintw(win_.get(), next_coords.second, next_coords.first, "o");
    wrefresh(win_.get());
}

void Window::clear_pos(const std::pair<unsigned, unsigned>& coords)
{
    std::lock_guard<std::mutex> l_g(mtx_);
    mvwprintw(win_.get(), coords.second, coords.first, " ");
    wrefresh(win_.get());
}

void Window::light_up_upper_pa(const short color)
{
    std::lock_guard<std::mutex> l_g(mtx_);
    wattron(win_.get(), COLOR_PAIR(color));
    place_upper_pa();
    wattroff(win_.get(), COLOR_PAIR(color));
    wrefresh(win_.get());
}

void Window::light_up_lower_pa(const short color)
{
    std::lock_guard<std::mutex> l_g(mtx_);
    wattron(win_.get(), COLOR_PAIR(color));
    place_lower_pa();
    wattroff(win_.get(), COLOR_PAIR(color));
    wrefresh(win_.get());
}

Window::~Window() {}