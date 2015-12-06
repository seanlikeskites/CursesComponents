#include "Curses.hpp"
#include <array>

Curses::Curses()
{
    initscr();
    start_color();

    std::array <Colour, 8> colours {{Colour::black,
                                     Colour::red,
                                     Colour::green,
                                     Colour::yellow,
                                     Colour::blue,
                                     Colour::magenta,
                                     Colour::cyan,
                                     Colour::white}};

    for (auto background : colours)
    {
        for (auto foreground : colours)
        {
            ColourPair pairIndex = getColourPairIndex (background, foreground);
            init_pair (pairIndex, static_cast <short> (foreground), static_cast <short> (background));
        }
    }
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

int Curses::getScreenWidth()
{
    return COLS;
}

int Curses::getScreenHeight()
{
    return LINES;
}

Curses::ColourPair Curses::getColourPairIndex (Colour backgroundColour, Colour foregroundColour)
{
    return static_cast <short> (backgroundColour) + static_cast <short> (foregroundColour) * 8;
}

Window::Window (int x, int y, int width, int height)
    : window (newwin (height, width, y, x), delwin),
      panel (new_panel (window.get()), del_panel),
      backgroundColour (Curses::Colour::black),
      foregroundColour (Curses::Colour::white)
{
    setColours (backgroundColour, foregroundColour);
}

Window::Window (Window &&other)
    : window (std::move (other.window)),
      panel (new_panel (window.get()), del_panel),
      backgroundColour (other.backgroundColour),
      foregroundColour (other.foregroundColour)
{
}

Window& Window::operator= (Window &&rhs)
{
    window = std::move (rhs.window);
    panel = Curses::PanelPointer (new_panel (window.get()), del_panel);

    backgroundColour = rhs.backgroundColour;
    foregroundColour = rhs.foregroundColour;

    return *this;
}

Window::~Window()
{
}

void Window::move (int x, int y)
{
    move_panel (panel.get(), y, x);
}

void Window::resize (int x, int y, int width, int height)
{
    Curses::WindowPointer tempWindow (newwin (height, width, y, x), delwin);
    replace_panel (panel.get(), tempWindow.get());
    window = std::move (tempWindow);
}

void Window::hide()
{
    hide_panel (panel.get());
}

void Window::show()
{
    show_panel (panel.get());
}

void Window::printCharacter (const chtype character)
{
    waddch (window.get(), character);
}

void Window::printCharacter (const chtype character, int x, int y)
{
    mvwaddch (window.get(), y, x, character);
}

void Window::printString (const std::string &string)
{
    waddstr (window.get(), string.c_str());
}

void Window::printString (const std::string &string, int x, int y)
{
    mvwaddstr (window.get(), y, x, string.c_str());
}

void Window::printDouble (double value)
{
    wprintw (window.get(), "%.2f", value);
}

void Window::printDouble (double value, int x, int y)
{
    mvwprintw (window.get(), y, x, "%.2f", value);
}

void Window::printInteger (int value)
{
    wprintw (window.get(), "%d", value);
}

void Window::printInteger (int value, int x, int y)
{
    mvwprintw (window.get(), y, x, "%d", value);
}

Window::VideoAttributes Window::getVideoAttributes() const
{
    VideoAttributes attributes;
    wattr_get (window.get(), &attributes.attributes, &attributes.colourPair, nullptr);

    return attributes;
}

void Window::setVideoAttributes (const VideoAttributes &attributes)
{
    wattr_set (window.get(), attributes.attributes, attributes.colourPair, nullptr);
}

void Window::setBackgroundColour (Curses::Colour newBackgroundColour)
{
    setColours (newBackgroundColour, foregroundColour);
}

void Window::setForegroundColour (Curses::Colour newForegroundColour)
{
    setColours (backgroundColour, newForegroundColour);
}

void Window::setColours (Curses::Colour newBackgroundColour, Curses::Colour newForegroundColour)
{
    backgroundColour = newBackgroundColour;
    foregroundColour = newForegroundColour;
    wattron (window.get(), COLOR_PAIR (Curses::getInstance().getColourPairIndex (backgroundColour, foregroundColour)));
}

void Window::setBold (bool setting)
{
    if (setting)
    {
        wattron (window.get(), A_BOLD);
    }
    else
    {
        wattroff (window.get(), A_BOLD);
    }
}

void Window::setUnderline (bool setting)
{
    if (setting)
    {
        wattron (window.get(), A_UNDERLINE);
    }
    else
    {
        wattroff (window.get(), A_UNDERLINE);
    }
}
