#include <ncurses.h>
#include "../include/window.hpp"
#include <locale.h>

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "");
    nodelay(stdscr, 0);
    Window win = Window();
    win.release_the_hounds();
    return 0;
}