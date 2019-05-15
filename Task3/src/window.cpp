#include "../include/window.hpp"

Window::Window()
    : win_(newwin(getmaxy(stdscr), getmaxx(stdscr), 0, 0), [](WINDOW* w) {
        delwin(w);
        endwin();
    })
{
    draw_foreground();
}

void Window::draw_foreground()
{
    std::lock_guard<std::mutex> l_g(mtx_);
    box(win_.get(), 0, 0);
    place_connections();
    place_hangars();
    wrefresh(win_.get());
}

void Window::place_connections()
{
    const int x_start = 17;
    const int x_end = max_x() - 17;
    const int line_len = x_end - 16;
    mvwhline(win_.get(), max_y() - 3, x_start, ACS_HLINE, line_len);

    // Middle
    const int half_len = max_x() / 2;
    const int mid_start = half_len - 3;
    const int mid_len = 6;
    mvwhline(win_.get(), max_y() - 7, mid_start, ACS_HLINE, mid_len);

    // Left
    const int left_len = mid_start - runway_width - x_start;
    mvwhline(win_.get(), max_y() - 7, x_start, ACS_HLINE, left_len);

    // Right
    const int right_start = mid_start + mid_len + runway_width;
    const int right_len = x_end - (mid_start + mid_len + runway_width - 1);
    mvwhline(win_.get(), max_y() - 7, right_start, ACS_HLINE, right_len);
    place_runways(x_start + left_len, mid_start + mid_len);
}

void Window::place_runways(int startx1, int startx2)
{
    const int runway_start = 10;
    const int runway_end = max_y() - 7;
    const int runway_len = runway_end - runway_start;

    mvwvline(win_.get(), runway_start, startx1, ACS_VLINE, runway_len);
    mvwvline(win_.get(), runway_start, startx1 + runway_width - 1, ACS_VLINE, runway_len);
    mvwaddch(win_.get(), runway_end, startx1, ACS_LRCORNER);
    mvwaddch(win_.get(), runway_end, startx1 + runway_width - 1, ACS_LLCORNER);

    mvwvline(win_.get(), runway_start, startx2, ACS_VLINE, runway_len);
    mvwvline(win_.get(), runway_start, startx2 + runway_width - 1, ACS_VLINE, runway_len);
    mvwaddch(win_.get(), runway_end, startx2, ACS_LRCORNER);
    mvwaddch(win_.get(), runway_end, startx2 + runway_width - 1, ACS_LLCORNER);
}

void Window::place_hangars()
{
    ncurses_rectangle(max_y() - 9, 1, max_y() - 1, 15);
    ncurses_rectangle(max_y() - 9, max_x() - 15, max_y() - 1, max_x() - 1);
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