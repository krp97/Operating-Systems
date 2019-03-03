#include <ncurses.h>
#include "../include/window.hpp"

int main(int argc, char** argv)
{
    Window win = Window();
    win.release_the_hounds();
    return 0;
}