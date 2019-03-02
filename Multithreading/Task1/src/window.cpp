#include "../include/window.hpp"

Window::Window()
{
    initscr();
    int term_width  = getmaxx(stdscr);
    int term_height = getmaxy(stdscr);
    WINDOW* win     = newwin(term_height, term_width, 0, 0);
    // window_         = std::make_shared<WINDOW>(win);
    refresh();
    box(win, 0, 0);
    wrefresh(win);
}

Window::Window(int h_lines, int v_lines, int x_start, int y_start)
{
    initscr();
    WINDOW* win = newwin(v_lines, h_lines, x_start, y_start);
    // window_     = std::make_shared<WINDOW>(win);
    box(win, 0, 0);
    wrefresh(win);
}

Window::~Window() { endwin(); }