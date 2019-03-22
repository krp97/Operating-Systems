#include "../include/window.hpp"

Window::Window() : shutdown_flag_ {false}
{
    initscr();
    curs_set(0);
    int term_width  = getmaxx(stdscr);
    int term_height = getmaxy(stdscr);
    window_         = newwin(term_height, term_width, 0, 0);
    box(window_, 0, 0);
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

void Window::start()
{
    key_watcher_ = std::thread([&]() { pressed_exit(); });

    while (!shutdown_flag_.load())
    {
        auto ptr =
            std::make_unique<Ball>(std::chrono::milliseconds(40), window_);
        ball_vec.push_back(std::move(ptr));
        ball_vec.back()->start();
        wait_n_check_shutdwn(std::chrono::milliseconds(3000));
    }
}

void Window::wait_n_check_shutdwn(std::chrono::milliseconds wait_time)
{
    auto start    = std::chrono::high_resolution_clock::now();
    auto end      = start;
    auto duration = std::chrono::duration<double>(end - start);
    while (!shutdown_flag_.load() && duration < wait_time)
    {
        end      = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration<double>(end - start);
    }
}

void Window::pressed_exit()
{
    while (getch() != 27)
        ;  // 27 - key code for "esc"
    shutdown_flag_.store(true);
}

Window::~Window()
{
    key_watcher_.join();
    ball_vec.clear();
    endwin();
}