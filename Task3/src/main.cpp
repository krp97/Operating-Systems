#include <locale.h>
#include <ncurses.h>
#include <chrono>
#include <thread>
#include "../include/control_tower.hpp"
#include "../include/flight_generator.hpp"
#include "../include/window.hpp"

int main()
{
    initscr();
    start_color();
    Window win;
    Control_Tower ct;
    Flight_Generator fg =
        Flight_Generator(win, ct, 1, std::chrono::milliseconds(10));
    ct.idle_func();
    return 0;
}
