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
    Ball& operator=(Ball&& rhs) = default;
    Ball(Ball&& ball)           = default;
    ~Ball();

    void move(const Direction&);
    void move_random();
    void idle_func();

   private:
    float speed;
    Direction direction;
    std::shared_ptr<WINDOW> window;
    std::thread t;
};