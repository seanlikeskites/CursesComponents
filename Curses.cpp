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
    : window (newwin (height, width, y, x), delwin),
      panel (new_panel (window.get()), del_panel)
{
}

Window::Window (const Window &other)
    : window (dupwin (other.window.get()), delwin),
      panel (new_panel (other.window.get()), del_panel)
{
}

Window& Window::operator= (const Window &rhs)
{
    window = Curses::WindowPointer (dupwin (rhs.window.get()), delwin);
    panel = Curses::PanelPointer (new_panel (window.get()), del_panel);
    return *this;
}

Window::Window (Window &&other)
    : window (std::move (other.window)),
      panel (new_panel (window.get()), del_panel)
{
}

Window& Window::operator= (Window &&rhs)
{
    window = std::move (rhs.window);
    panel = Curses::PanelPointer (new_panel (window.get()), del_panel);

    return *this;
}

Window::~Window()
{
}

void Window::hide()
{
    hide_panel (panel.get());
}

void Window::show()
{
    show_panel (panel.get());
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
