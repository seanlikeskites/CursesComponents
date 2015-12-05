#include "Curses.hpp"

Curses::Curses()
{
    initscr();
}

Curses::~Curses()
{
    endwin();
}

Curses& Curses::getInstance()
{
    static Curses instance;
    return instance;
}

Window Curses::createWindow (int x, int y, int width, int height)
{
    return Window (x, y, width, height);
}

Window::Window (int x, int y, int width, int height)
    : window (newwin (height, width, y, x), delwin)
{
}

Window::Window (Window &&other)
    : window (std::move (other.window))
{
}

Window& Window::operator= (Window &&rhs)
{
    window = std::move (rhs.window);

    return *this;
}

Window::~Window()
{
}

void Window::addCharacter (const chtype character)
{
    waddch (window.get(), character);
}

void Window::addCharacter (const chtype character, int x, int y)
{
    mvwaddch (window.get(), y, x, character);
}

void Window::addString (const char *string)
{
    waddstr (window.get(), string);
}

void Window::addString (const char *string, int x, int y)
{
    mvwaddstr (window.get(), y, x, string);
}

void Window::waitForInput()
{
    wgetch (window.get());
}
