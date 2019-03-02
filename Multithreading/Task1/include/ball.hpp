#include <ncurses.h>
#include <functional>
#include <thread>
#include "direction.hpp"

class Ball
{
   public:
    Ball() = default;
    Ball(float, std::shared_ptr<WINDOW>, Direction dir);
    Ball& operator=(const Ball& rhs) = delete;
    Ball(const Ball& ball)           = delete;
    Ball& operator=(Ball&& rhs) noexcept = default;
    Ball(Ball&& ball) noexcept           = default;
    ~Ball();

    void move();
    void idle_func();

   private:
    float speed_;
    Direction direction_;
    std::shared_ptr<WINDOW> window_;
    std::thread t_;
};