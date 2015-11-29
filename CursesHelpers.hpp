#ifndef CURSES_HELPERS_HPP_INCLUDED
#define CURSES_HELPERS_HPP_INCLUDED

#include <memory>
#include <curses.h>

class Curses
{
public:
    Curses()
    {
        initscr();
    }

    ~Curses()
    {
        endwin();
    }

    using WindowPointer = std::unique_ptr <WINDOW, int(*)(WINDOW*)>;

    static WindowPointer createWindow (int numLines, int numCols, int y, int x)
    {
        WindowPointer newWindow (newwin (numLines, numCols, y, x), delwin);
        return newWindow;
    }
};

#endif // CURSES_HELPERS_HPP_INCLUDED
