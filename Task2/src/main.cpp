#include <locale.h>
#include <ncurses.h>
#include "../include/main_handler.hpp"

int main(int argc, char** argv)
{
    initscr();
    curs_set(0);
    nodelay(stdscr, 0);
    Main_Handler m_h = Main_Handler();
    m_h.start();
    return 0;
}
