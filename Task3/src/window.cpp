#include "../include/window.hpp"

// Runway start point is a horizontal half point, shited to the left or right
// by: sum(half the runway width, half the middle connector width).

Window::Window()
    : LEFT_RUNWAY_START {getmaxx(stdscr) / 2 - (3 + RUNWAY_WIDTH / 2),
                         getmaxy(stdscr) - 10},
      RIGHT_RUNWAY_START {getmaxx(stdscr) / 2 + (3 + RUNWAY_WIDTH / 2),
                          getmaxy(stdscr) - 10},
      HANGAR_OUT {0, getmaxy(stdscr) - (BOTTOM_PADDING + 2)},
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
}

void Window::draw_foreground()
{
    init_pairs();
    std::lock_guard<std::mutex> l_g(mtx_);
    box(win_.get(), 0, 0);
    place_connections();
    place_hangar();
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
    mvwhline(win_.get(), max_y() - TOP_PADDING, mid_start, ACS_HLINE,
             mid_len);

    // Left
    const int left_len = mid_start - RUNWAY_WIDTH - x_start;
    mvwhline(win_.get(), max_y() - TOP_PADDING, x_start, ACS_HLINE,
             left_len);
    wattroff(win_.get(), COLOR_PAIR(WHITE));
    place_runways(x_start + left_len, mid_start + mid_len);
}

void Window::place_runways(int startx1, int startx2)
{
    constexpr int runway_start = 10;
    const int runway_end       = max_y() - TOP_PADDING;
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
             runway_len + 6);
    mvwaddch(win_.get(), runway_end, startx2, ACS_LRCORNER);
    mvwaddch(win_.get(), max_y() - BOTTOM_PADDING, max_x() - 2,
             ACS_LRCORNER);
    wattroff(win_.get(), COLOR_PAIR(WHITE));
}

void Window::place_hangar()
{
    wattron(win_.get(), COLOR_PAIR(RED));
    ncurses_rectangle(max_y() - 9, 1, max_y() - 1, 15);
    mvwprintw(win_.get(), max_y() - 5, 5, "Hangar");
    wattroff(win_.get(), COLOR_PAIR(RED));
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

Window::~Window() {}