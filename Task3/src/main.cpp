#include <locale.h>
#include <ncurses.h>
#include <chrono>
#include <thread>
#include "../include/control_tower.hpp"
#include "../include/flight_generator.hpp"
#include "../include/window.hpp"
void key_poller(Control_Tower& ct)
{
    char ch;
    while ((ch = getch()) != 27)
    {
        if (ch == 'b')
            ct.break_runway();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    ct.shutdown();
}

int main()
{
    initscr();
    start_color();
    nodelay(stdscr, 0);
    Window win;
    Control_Tower ct(win);
    std::thread key_watcher([&]() { key_poller(ct); });
    Flight_Generator fg =
        Flight_Generator(win, ct, 0.5, std::chrono::milliseconds(10));
    ct.idle_func();
    key_watcher.join();
    int i = 0;
    return 0;
}
