#include "Curses.hpp"

int main()
{
    Curses::Instance curses = Curses::getInstance();

    Window window = curses.createWindow (2, 2, 10, 10);
    Window window2 = curses.createWindow (4, 4, 10, 10);

    window.addCharacter ('F');
    window.addCharacter ('C', 5, 5);

    window.addString ("Hello World!");
    window.addString ("Not me!", 3, 0);

    window.hide();

    update_panels();
    doupdate();

    getch();

    return 0;
}
