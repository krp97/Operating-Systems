#include <locale.h>
#include <ncurses.h>
#include <chrono>
#include <thread>
#include "../include/airplane.hpp"
#include "../include/incoming_airplane.hpp"
#include "../include/outgoing_airplane.hpp"
#include "../include/window.hpp"

int main()
{
    initscr();
    start_color();
    Window win;
    //Airplane* oa = new Outgoing_Airplane(std::chrono::milliseconds(40), win,
                                        // win.HANGAR_OUT);
    //Airplane* ia = new Incoming_Airplane(std::chrono::milliseconds(40), win,
                                         //win.LEFT_RUNWAY_END);
    getchar();
    //delete oa, ia;
    return 0;
}
