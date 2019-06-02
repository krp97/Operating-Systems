#include "../include/window.hpp"
#include "../include/utils.hpp"

Window::Window()
    : LEFT_RUNWAY_START {getmaxx(stdscr) -
                             (2 * RUNWAY_WIDTH + RUNWAY_CONNECTION_WIDTH +
                              RIGHT_PADDING) +
                             RUNWAY_WIDTH / 2,
                         getmaxy(stdscr) - BOTTOM_PADDING - 4},
      RIGHT_RUNWAY_START {
          LEFT_RUNWAY_START.first + RUNWAY_WIDTH + RUNWAY_CONNECTION_WIDTH,
          getmaxy(stdscr) - BOTTOM_PADDING - 2},
      PASSENGER_STOP {static_cast<size_t>(getmaxx(stdscr) / 2)},
      UPPER_LANE_Y {
          static_cast<size_t>(getmaxy(stdscr) - (BOTTOM_PADDING + 4))},
      LOWER_LANE_Y {
          static_cast<size_t>(getmaxy(stdscr) - (BOTTOM_PADDING + 2))},
      HANGAR_OUT_LOWER {
          16, static_cast<size_t>(getmaxy(stdscr) - (BOTTOM_PADDING + 2))},
      HANGAR_OUT_UPPER {
          16, static_cast<size_t>(getmaxy(stdscr) - (BOTTOM_PADDING + 4))},
      LEFT_RUNWAY_END {LEFT_RUNWAY_START.first, 16},
      RIGHT_RUNWAY_END {RIGHT_RUNWAY_START.first, 16},
      win_(newwin(getmaxy(stdscr), getmaxx(stdscr), 0, 0), [](WINDOW* w) {
          delwin(w);
          endwin();
      })

{
    use_default_colors();
    curs_set(false);
}

void Window::init_pairs()
{
    init_color(COLOR_GREEN, 0, 700, 0);
    init_color(COLOR_RED, 700, 0, 0);

    init_pair(WHITE, COLOR_WHITE, -1);
    init_pair(RED, COLOR_RED, -1);
    init_pair(BLUE, COLOR_BLUE, -1);
    init_pair(GREEN, COLOR_GREEN, -1);
    init_pair(PINK, COLOR_MAGENTA, -1);
    init_pair(YELLOW, COLOR_YELLOW, -1);
}

void Window::update_loading_bar(const short stat_v_pos, const int pegs)
{
    std::lock_guard<std::mutex> l_g(mtx_);
    int dashes = 15 - pegs;
    std::string bar_string;

    if (dashes >= 0)
    {
        wattron(win_.get(), COLOR_PAIR(YELLOW));
        mvwprintw(win_.get(), stat_v_pos, 18, "Fixing  ");
        bar_string = std::string("[" + std::string(pegs, '#') +
                                 std::string(dashes, '-') + "]   ");
        mvwprintw(win_.get(), stat_v_pos, 26, bar_string.c_str());
        wrefresh(win_.get());
        wattroff(win_.get(), COLOR_PAIR(YELLOW));
    }
    else
    {
        wattron(win_.get(), COLOR_PAIR(GREEN));
        bar_string = std::string(17, ' ');
        mvwprintw(win_.get(), stat_v_pos, 26, bar_string.c_str());
        mvwprintw(win_.get(), stat_v_pos, 18, "Free      ");
        wattroff(win_.get(), COLOR_PAIR(GREEN));
    }
}

void Window::draw_foreground()
{
    init_pairs();
    box(win_.get(), 0, 0);
    draw_title();
    draw_keyfuncs();
    draw_stats();
    place_connections();
    place_hangar();

    // These have to be manually locked, because they're called by the
    // public methods, which already hold the lock.
    std::unique_lock<std::mutex> l_g(mtx_);
    place_upper_pa();
    place_lower_pa();
    wrefresh(win_.get());
}

void Window::draw_title()
{
    std::lock_guard<std::mutex> l_g(mtx_);
    wattron(win_.get(), COLOR_PAIR(PINK));
    ncurses_rectangle(2, 2, TOP_PADDING - 6, max_x() - 2);
    mvwprintw(win_.get(), 3, (max_x() / 2) - 8, "Airport Simulator");
    wattroff(win_.get(), COLOR_PAIR(PINK));
    wrefresh(win_.get());
}

void Window::draw_stats()
{
    std::unique_lock<std::mutex> l_g(mtx_);
    wattron(win_.get(), COLOR_PAIR(WHITE));
    mvwprintw(win_.get(), 5, 4, "Left  runway: ");
    mvwprintw(win_.get(), 6, 4, "Right runway: ");
    mvwprintw(win_.get(), 7, 13, "PA1: ");
    mvwprintw(win_.get(), 8, 13, "PA2: ");
    wattroff(win_.get(), COLOR_PAIR(WHITE));
    l_g.unlock();
    change_status(LEFT_RUNWAY_STAT, "Free", GREEN);
    change_status(RIGHT_RUNWAY_STAT, "Free", GREEN);
    change_status(PA1_STAT, "Free", GREEN);
    change_status(PA2_STAT, "Free", GREEN);
}

void Window::draw_keyfuncs()
{
    std::lock_guard<std::mutex> l_g(mtx_);
    wattron(win_.get(), COLOR_PAIR(WHITE));
    mvwprintw(win_.get(), 5, max_x() - 21, "Hotkeys:");
    mvwprintw(win_.get(), 6, max_x() - 21, "b - Break a runway");
    mvwprintw(win_.get(), 7, max_x() - 21, "Esc - Exit");
    wattroff(win_.get(), COLOR_PAIR(WHITE));
    wrefresh(win_.get());
}

void Window::change_status(const short stat, std::string status,
                           const short color)
{
    wattron(win_.get(), COLOR_PAIR(color));
    mvwprintw(win_.get(), stat, 18, status.c_str());
    wattroff(win_.get(), COLOR_PAIR(color));
    wrefresh(win_.get());
}

void Window::free_pa(const std::pair<size_t, size_t> passenger_area)
{
    std::lock_guard<std::mutex> l_g(mtx_);
    if (passenger_area == std::make_pair(PASSENGER_STOP, LOWER_LANE_Y))
    {
        change_status(PA2_STAT, "Free    ", GREEN);
        wattron(win_.get(), COLOR_PAIR(WHITE));
        place_lower_pa();
        wattroff(win_.get(), COLOR_PAIR(WHITE));
    }
    else
    {
        change_status(PA1_STAT, "Free    ", GREEN);
        wattron(win_.get(), COLOR_PAIR(WHITE));
        place_upper_pa();
        wattroff(win_.get(), COLOR_PAIR(WHITE));
    }
    wrefresh(win_.get());
}

void Window::occupy_pa(const std::pair<size_t, size_t> passenger_area)
{
    std::lock_guard<std::mutex> l_g(mtx_);
    if (passenger_area == std::make_pair(PASSENGER_STOP, LOWER_LANE_Y))
    {
        change_status(PA2_STAT, "Occupied", BLUE);
        wattron(win_.get(), COLOR_PAIR(BLUE));
        place_lower_pa();
        wattroff(win_.get(), COLOR_PAIR(BLUE));
    }
    else
    {
        change_status(PA1_STAT, "Occupied", BLUE);
        wattron(win_.get(), COLOR_PAIR(BLUE));
        place_upper_pa();
        wattroff(win_.get(), COLOR_PAIR(BLUE));
    }
    wrefresh(win_.get());
}

void Window::free_runway(const std::pair<size_t, size_t> runway_start)
{
    std::lock_guard<std::mutex> l_g(mtx_);
    if (runway_start == LEFT_RUNWAY_START)
        change_status(LEFT_RUNWAY_STAT, "Free    ", GREEN);
    else
        change_status(RIGHT_RUNWAY_STAT, "Free    ", GREEN);
}

void Window::occupy_runway(const std::pair<size_t, size_t> runway_start)
{
    std::lock_guard<std::mutex> l_g(mtx_);
    if (runway_start == LEFT_RUNWAY_START)
        change_status(LEFT_RUNWAY_STAT, "Occupied", BLUE);
    else
        change_status(RIGHT_RUNWAY_STAT, "Occupied", BLUE);
}

void Window::break_runway(const std::pair<size_t, size_t> runway_start)
{
    std::lock_guard<std::mutex> l_g(mtx_);
    if (runway_start == LEFT_RUNWAY_START)
        mvwprintw(win_.get(), LEFT_RUNWAY_STAT, 27, "<- clearing lane");
    else
        mvwprintw(win_.get(), RIGHT_RUNWAY_STAT, 27, "<- clearing lane");
    wrefresh(win_.get());
}

void Window::place_connections()
{
    std::lock_guard<std::mutex> l_g(mtx_);
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
    std::lock_guard<std::mutex> l_g(mtx_);
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

Window::~Window() {}