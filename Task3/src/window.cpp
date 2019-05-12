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
    place_runways();
    place_hangars();
    wrefresh(win_.get());
}

void Window::place_runways()
{
    mvwhline(win_.get(), 3, 25, ACS_HLINE, 60);
    mvwhline(win_.get(), 6, 25, ACS_HLINE, 60);

    mvwhline(win_.get(), max_y() - 6, 25, ACS_HLINE, 60);
    mvwhline(win_.get(), max_y() - 3, 25, ACS_HLINE, 60);
}

void Window::place_hangars()
{
    ncurses_rectangle(1, 1, 8, 7);
    ncurses_rectangle(max_y() - 8, 1, max_y() - 1, 7);
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