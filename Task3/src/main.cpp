#include "../include/window.hpp"
#include <locale.h>
#include <ncurses.h>

int main()
{
    initscr();
    start_color();
    Window win;
    getchar();
    return 0;
}
