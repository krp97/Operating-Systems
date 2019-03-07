#include <ncurses.h>
#include "../include/window.hpp"

int main(int argc, char** argv)
{
    nodelay(stdscr, TRUE);
    Window win = Window();
    win.release_the_hounds();
    return 0;
}