#include "../include/main_handler.hpp"
#include <locale.h>
#include <ncurses.h>

int main(int argc, char** argv)
{
    initscr();
    curs_set(0);
    nodelay(stdscr, 0);
    Main_Handler m_h;
    m_h.start();
    return 0;
}
