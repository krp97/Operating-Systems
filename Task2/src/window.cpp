#include "../include/window.hpp"

Window::Window()
    : win_(newwin(getmaxy(stdscr), getmaxx(stdscr), 0, 0), [](WINDOW* w) {
          delwin(w);
          endwin();
      })
{
}

void Window::move_on_screen(std::pair<unsigned, unsigned>& prev_coords,
                            std::pair<unsigned, unsigned>& next_coords)
{
    std::lock_guard<std::mutex> l_g(mtx_);
    mvwprintw(win_.get(), prev_coords.second, prev_coords.first, " ");
    mvwprintw(win_.get(), next_coords.second, next_coords.first, "o");
    box(win_.get(), 0, 0);
    wrefresh(win_.get());
}

Window::~Window() {}