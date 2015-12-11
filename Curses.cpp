#include "Curses.hpp"
#include <array>
#include <string>
#include <cmath>
#include <cstdlib>
#include "MathsTools.hpp"

Curses::Curses()
{
    initscr();
    keypad (stdscr, true);
    cbreak();
    noecho();
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
    return static_cast <short> (backgroundColour) + static_cast <short> (foregroundColour) * 8 + 1;
}

Window::Window (int x, int y, int widthInit, int heightInit)
    : width (widthInit), height (heightInit),
      window (newwin (height, width, y, x), delwin),
      panel (new_panel (window.get()), del_panel),
      backgroundColour (Curses::Colour::black),
      foregroundColour (Curses::Colour::white)
{
    setColours (backgroundColour, foregroundColour);
}

Window::Window (Window &&other)
    : width (other.width), height (other.height),
      window (std::move (other.window)),
      panel (new_panel (window.get()), del_panel),
      backgroundColour (other.backgroundColour),
      foregroundColour (other.foregroundColour)
{
}

Window& Window::operator= (Window &&rhs)
{
    width = rhs.width;
    height = rhs.height;

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

void Window::resize (int x, int y, int newWidth, int newHeight)
{
    Curses::WindowPointer tempWindow (newwin (height, newWidth, y, x), delwin);
    replace_panel (panel.get(), tempWindow.get());
    window = std::move (tempWindow);

    width = newWidth;
    height = newHeight;
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

void Window::drawLine (int startX, int startY, int endX, int endY, const chtype character)
{
    int xRange = endX - startX;
    int yRange = endY - startY;

    int x = startX;
    int y = startY;

    int *iterationDimension = nullptr;
    int iterationEnd = 0;
    int iterationIncrement = 0;

    int *otherDimension = nullptr;
    double otherDimensionDouble = 0.0;
    double otherIncrement = 0.0;

    if (abs (xRange) >= abs (yRange))
    {
        iterationDimension = &x;
        iterationEnd = endX;
        iterationIncrement = MathsTools::sign (xRange);

        otherDimension = &y;
        otherDimensionDouble = static_cast <double> (y);
        otherIncrement = static_cast <double> (yRange) / static_cast <double> (abs (xRange));
    }
    else
    {
        iterationDimension = &y;
        iterationEnd = endY;
        iterationIncrement = MathsTools::sign (yRange);

        otherDimension = &x;
        otherDimensionDouble = static_cast <double> (x);
        otherIncrement = static_cast <double> (xRange) / static_cast <double> (abs (yRange));
    }

    for (; *iterationDimension != iterationEnd + iterationIncrement; *iterationDimension += iterationIncrement)
    {
        mvwaddch (window.get(), y, x, character);
        otherDimensionDouble += otherIncrement;
        *otherDimension = round (otherDimensionDouble);
    }
}

void Window::drawEllipse (int x, int y, int width, int height, const chtype character)
{
    int alteredWidth = width - 1;
    int alteredHeight = height - 1;

    int plotX = 0;
    int plotY = 0;

    int *iterationDimension = nullptr;
    int *otherDimension = nullptr;

    double iterationScale = 0.0;
    double iterationOffset = 0.0;

    double otherScale = 0.0;
    double otherOffset = 0.0;

    if (alteredWidth >= alteredHeight)
    {
        iterationDimension = &plotX;
        otherDimension = &plotY;

        iterationScale = alteredWidth / 2.0;
        iterationOffset = x + iterationScale;

        otherScale = alteredHeight / 2.0;
        otherOffset = y + otherScale;
    }
    else
    {
        iterationDimension = &plotY;
        otherDimension = &plotX;

        iterationScale = alteredHeight / 2.0;
        iterationOffset = y + iterationScale;

        otherScale = alteredWidth / 2.0;
        otherOffset = x + otherScale;
    }

    double iterationIncrement = 1.0 / (width * height);

    for (double i = 0.0; i <= 1.0; i += iterationIncrement / 2.0)
    {
        auto printCircleLine = [iterationDimension,
                                iterationScale, iterationOffset, 
                                otherDimension,
                                otherScale, otherOffset,
                                &plotX, &plotY,
                                character,
                                this] (double xValue)
                               {
                                   *iterationDimension = round (xValue * iterationScale + iterationOffset);

                                   double circleLine = pow (1 - xValue * xValue, 0.5);

                                   *otherDimension = round (circleLine * otherScale + otherOffset);
                                   printCharacter (character, plotX, plotY);

                                   *otherDimension = round (-circleLine * otherScale + otherOffset);
                                   printCharacter (character, plotX, plotY);
                               };
    
        printCircleLine (i);
        printCircleLine (-i);
    }
}

void Window::printInteger (int value, int x, int y)
{
    mvwprintw (window.get(), y, x, "%d", value);
}

void Window::fillAll(const chtype character)
{
    for (int x = 0; x < width; ++x)
    {
        drawLine (x, 0, x, height - 1, character);
    }
}

void Window::clear()
{
    werase (window.get());
}

int Window::getWidth()
{
    return width;
}

int Window::getHeight()
{
    return height;
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
