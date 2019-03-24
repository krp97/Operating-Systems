#include "../include/main_handler.hpp"

void Main_Handler::start()
{
    key_watcher_ = std::thread([&]() { pressed_exit(); });

    while (!shutdown_flag_.load())
    {
        auto ptr = std::make_unique<Ball>(std::chrono::milliseconds(40), win_);
        ball_vec.push_back(std::move(ptr));
        ball_vec.back()->start();
        wait_n_check_shutdwn(std::chrono::milliseconds(3000));
    }
}

void Main_Handler::wait_n_check_shutdwn(std::chrono::milliseconds wait_time)
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

void Main_Handler::pressed_exit()
{
    while (getch() != 27)
        ;  // 27 - key code for "esc"
    shutdown_flag_.store(true);
}

Main_Handler::~Main_Handler()
{
    key_watcher_.join();
    ball_vec.clear();
}