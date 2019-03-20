#include <locale.h>
#include <ncurses.h>
#include "../include/window.hpp"

int main(int argc, char** argv)
{
    nodelay(stdscr, 0);
    Window win = Window();
    win.start();
    return 0;
}
