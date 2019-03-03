#include "../include/window.hpp"

Window::Window()
{
    initscr();
    int term_width  = getmaxx(stdscr);
    int term_height = getmaxy(stdscr);
    window_         = newwin(term_height, term_width, 0, 0);
    // box(window_, 0, 0);
    wrefresh(window_);
}

Window::Window(int h_lines, int v_lines, int x_start, int y_start)
{
    initscr();
    window_ = newwin(v_lines, h_lines, x_start, y_start);
    box(window_, 0, 0);
    wrefresh(window_);
}

Window& Window::operator=(Window&& other)
{
    if (window_ != other.window_)
    {
        delwin(window_);
        window_       = other.window_;
        other.window_ = nullptr;
    }
    return *this;
}

Window::Window(Window&& other)
{
    window_       = nullptr;
    window_       = other.window_;
    other.window_ = nullptr;
}

void Window::release_the_hounds()
{
    while (true)
    {
        balls.push_back(std::unique_ptr<Ball>(new Ball(
            std::chrono::milliseconds(1000), window_, Direction({1, 1}))));
        std::cout << "created\n";
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

void Window::stop_all() {}

Window::~Window()
{
    stop_all();
    endwin();
}