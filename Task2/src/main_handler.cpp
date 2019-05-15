#include "../include/main_handler.hpp"

void Main_Handler::start()
{
    key_watcher_ = std::thread([&]() { pressed_exit(); });

    while (!shutdown_flag_.load()) {
        auto ptr = std::make_unique<Ball>(std::chrono::milliseconds(40), win_);
        ball_vec.push_back(std::move(ptr));
        ball_vec.back()->start();
        std::mutex mtx;
        std::unique_lock<std::mutex> lk(mtx);
        cv_.wait_for(lk, std::chrono::milliseconds(3000), [&]() { return shutdown_flag_.load(); });
    }
}

void Main_Handler::pressed_exit()
{
    while (getch() != 27)
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    shutdown_flag_.store(true);
    cv_.notify_all();
}

Main_Handler::~Main_Handler()
{
    key_watcher_.join();
    for (auto& b : ball_vec)
        b->stop();
    ball_vec.clear();
}