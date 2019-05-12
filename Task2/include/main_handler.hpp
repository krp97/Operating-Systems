#include <atomic>
#include <chrono>
#include <memory>
#include <thread>

#include "ball.hpp"
#include "window.hpp"

class Main_Handler
{
   public:
    ~Main_Handler();
    void start();

   private:
    std::atomic<bool> shutdown_flag_ {false};
    std::thread key_watcher_;
    std::vector<std::unique_ptr<Ball>> ball_vec;
    Window win_;

    void pressed_exit();
    void wait_n_check_shutdwn(std::chrono::milliseconds);
};