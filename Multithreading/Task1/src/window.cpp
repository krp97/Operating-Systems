#include "../include/window.hpp"

Window::Window() : shutdown_flag_ {false}
{
    initscr();
    int term_width  = getmaxx(stdscr);
    int term_height = getmaxy(stdscr);
    window_         = newwin(term_height, term_width, 0, 0);
    box(window_, 0, 0);
    curs_set(0);
    wrefresh(window_);
}

Window::Window(int h_lines, int v_lines, int x_start, int y_start)
    : shutdown_flag_ {false}
{
    initscr();
    curs_set(0);
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
    while (!shutdown_flag_)
    {
        balls.push_back(std::unique_ptr<Ball>(new Ball(
            std::chrono::milliseconds(40), window_, Direction({1, 1}))));
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

void Window::stop_all()
{
    for (auto& ball : balls)
        ball->request_stop();
}

Window::~Window()
{
    stop_all();
    endwin();
}