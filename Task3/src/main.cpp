#include <locale.h>
#include <ncurses.h>
#include <chrono>
#include <iostream>
#include <string>
#include "../include/airplane.hpp"
#include "../include/outgoing_airplane.hpp"
#include "../include/window.hpp"
#include <thread>
int main()
{
    initscr();
    start_color();
    Window win;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    Airplane* oa = new Outgoing_Airplane(std::chrono::milliseconds(3000), win,
                                         win.HANGAR_OUT);
    getchar();
    delete oa;
    return 0;
}
