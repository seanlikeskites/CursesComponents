#include "Curses.hpp"
#include <array>
#include <cmath>
#include <cstdlib>
#include "MathsTools.hpp"

Curses::Curses()
{
    initscr();
    cbreak();
    noecho();
    start_color();
    keypad (stdscr, true);

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

    refresh();

    mainWindow.reset (new Window (0, 0, getScreenWidth(), getScreenHeight(), nullptr, nullptr, nullptr));
}

Curses::~Curses()
{
    endwin();
}

Curses::Instance Curses::getInstance()
{
    static Curses instance;
    return instance;
}

int Curses::getInputCharacter()
{
    return getch();
}

Window::Pointer Curses::createTopLevelWindow (int x, int y, int width, int height)
{
    Lock lock;
    return mainWindow->createChildWindow (x, y, width, height);
}

int Curses::getScreenWidth() const
{
    return COLS;
}

int Curses::getScreenHeight() const
{
    return LINES;
}

Curses::ColourPair Curses::getColourPairIndex (Colour backgroundColour, Colour foregroundColour)
{
    return static_cast <short> (backgroundColour) + static_cast <short> (foregroundColour) * 8 + 1;
}

void Curses::setCursor (Cursor newCursor)
{
    curs_set (static_cast <int> (newCursor));
}

void Curses::refreshScreen()
{
    Lock lock;
    doupdate();
}

Curses::Lock::Lock()
    : lock (Curses::protectionMutex)
{
}

Curses::Lock::~Lock()
{
}

std::recursive_mutex Curses::protectionMutex;

Window::Window (int x, int y, int widthInit, int heightInit, 
                Window::Pointer parentInit, Window::Pointer previousSiblingInit, Window::Pointer nextSiblingInit)
    : xPos (x), yPos (y),
      width (widthInit), height (heightInit),
      xStart (0), yStart (0),
      leftBound (0), rightBound (0),
      topBound (0), bottomBound (0),
      visible (true),
      window (newpad (height, width), delwin),
      blankWindow (newpad (height, width), delwin),
      backgroundColour (Curses::Colour::black),
      foregroundColour (Curses::Colour::white),
      parent (parentInit),
      previousSibling (previousSiblingInit),
      nextSibling (nextSiblingInit)
{
    updateBounds();
    setColours (backgroundColour, foregroundColour);
}

Window::~Window()
{
    Pointer pointerToThis;

    if (parent != nullptr)
    {
        if (parent->bottomChild.lock() == pointerToThis)
        {
            parent->bottomChild = nextSibling;
        }

        if (parent->topChild.lock() == pointerToThis)
        {
            parent->topChild = previousSibling;
        }
    }
}

Window::Pointer Window::createChildWindow (int x, int y, int width, int height)
{
    Curses::Lock lock;
    Window::Pointer newWindow (new Window (x + xPos, y + yPos, width, height,
                                           shared_from_this(), topChild.lock(), nullptr));

    if (bottomChild.lock() == nullptr)
    {
        bottomChild = newWindow;
    }

    if (topChild.lock() != nullptr)
    {
        topChild.lock()->nextSibling = newWindow;
    }

    topChild = newWindow;

    return newWindow;
}

void Window::move (int x, int y)
{
    Curses::Lock lock;
    xPos = x + parent->xPos;
    yPos = y + parent->yPos;
    updateBounds();
    refreshParent();
}

void Window::resize (int x, int y, int newWidth, int newHeight)
{
    Curses::Lock lock;
    window.reset (newpad (newHeight, newWidth));
    blankWindow.reset (newpad (newHeight, newWidth));

    xPos = x + parent->xPos;
    yPos = y + parent->yPos;
    width = newWidth;
    height = newHeight;
    updateBounds();
    refreshParent();
}

void Window::refresh ()
{
    Curses::Lock lock;

    if (visible)
    {
        pnoutrefresh (window.get(), yStart, xStart, topBound, leftBound, bottomBound, rightBound);

        if (bottomChild.lock() != nullptr)
        {
            bottomChild.lock()->refresh();
        }
    }
    else
    {
        pnoutrefresh (blankWindow.get(), yStart, xStart, topBound, leftBound, bottomBound, rightBound);
    }

    if (nextSibling.lock() != nullptr)
    {
        nextSibling.lock()->refresh();
    }
}

void Window::hide()
{
    visible = false;
    refresh();
}

void Window::show()
{
    visible = true;
    refresh();
}

void Window::printCharacter (const chtype character)
{
    Curses::Lock lock;
    waddch (window.get(), character);
}

void Window::printCharacter (const chtype character, int x, int y)
{
    Curses::Lock lock;
    mvwaddch (window.get(), y, x, character);
}

void Window::printString (const std::string &string)
{
    Curses::Lock lock;
    waddstr (window.get(), string.c_str());
}

void Window::printString (const std::string &string, int x, int y)
{
    Curses::Lock lock;
    mvwaddstr (window.get(), y, x, string.c_str());
}

void Window::printDouble (double value)
{
    Curses::Lock lock;
    wprintw (window.get(), "%.2f", value);
}

void Window::printDouble (double value, int x, int y)
{
    Curses::Lock lock;
    mvwprintw (window.get(), y, x, "%.2f", value);
}

void Window::printInteger (int value)
{
    Curses::Lock lock;
    wprintw (window.get(), "%d", value);
}

void Window::printInteger (int value, int x, int y)
{
    Curses::Lock lock;
    mvwprintw (window.get(), y, x, "%d", value);
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

    if (iterationIncrement == 0)
    {
        iterationIncrement = 1;
    }

    Curses::Lock lock;

    for (; *iterationDimension != iterationEnd + iterationIncrement; *iterationDimension += iterationIncrement)
    {
        printCharacter (character, x, y);
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

    Curses::Lock lock;

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

void Window::drawBox (int x, int y, int width, int height)
{
    Curses::Lock lock;
    int rightX = x + width - 1;
    int bottomY = y + height - 1;
    printCharacter (ACS_ULCORNER, x, y);
    printCharacter (ACS_LLCORNER, x, bottomY);
    printCharacter (ACS_URCORNER, rightX, y);
    printCharacter (ACS_LRCORNER, rightX, bottomY);

    int horizontalStart = x + 1;
    int horizontalEnd = rightX - 1;
    drawLine (horizontalStart, y, horizontalEnd, y, ACS_HLINE);
    drawLine (horizontalStart, bottomY, horizontalEnd, bottomY, ACS_HLINE);

    int verticalStart = y + 1;
    int verticalEnd = bottomY - 1;
    drawLine (x, verticalStart, x, verticalEnd, ACS_VLINE);
    drawLine (rightX, verticalStart, rightX, verticalEnd, ACS_VLINE);
}

void Window::fillAll(const chtype character)
{
    Curses::Lock lock;

    for (int x = 0; x < width; ++x)
    {
        drawLine (x, 0, x, height - 1, character);
    }
}

void Window::fillWithColour (Curses::Colour colour, bool setBackground)
{
    Curses::Lock lock;
    VideoAttributes attributeCache = getVideoAttributes();
    setBackgroundColour (colour);
    fillAll (' ');

    if (!setBackground)
    {
        setVideoAttributes (attributeCache);
    }
}

void Window::clear()
{
    Curses::Lock lock;
    werase (window.get());
}

int Window::getWidth() const
{
    return width;
}

int Window::getHeight() const
{
    return height;
}

Window::VideoAttributes Window::getVideoAttributes() const
{
    Curses::Lock lock;
    VideoAttributes attributes;
    wattr_get (window.get(), &attributes.attributes, &attributes.colourPair, nullptr);
    attributes.backgroundColour = backgroundColour;
    attributes.foregroundColour = foregroundColour;

    return attributes;
}

void Window::setVideoAttributes (const VideoAttributes &attributes)
{
    Curses::Lock lock;
    wattr_set (window.get(), attributes.attributes, attributes.colourPair, nullptr);
    backgroundColour = attributes.backgroundColour;
    foregroundColour = attributes.foregroundColour;
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

    Curses::Lock lock;
    wcolor_set (window.get(), Curses::getColourPairIndex (backgroundColour, foregroundColour), nullptr);
}

void Window::setBold (bool setting)
{
    Curses::Lock lock;
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
    Curses::Lock lock;
    if (setting)
    {
        wattron (window.get(), A_UNDERLINE);
    }
    else
    {
        wattroff (window.get(), A_UNDERLINE);
    }
}

void Window::updateBounds()
{
    xStart = 0;
    yStart = 0;
    leftBound = xPos;
    rightBound = xPos + width - 1;
    topBound = yPos;
    bottomBound = yPos + height - 1;

    if (parent != nullptr)
    {
        if (leftBound < parent->leftBound)
        {
            leftBound = parent->leftBound;
            xStart = leftBound - xPos;
        }

        if (rightBound > parent->rightBound)
        {
            rightBound = parent->rightBound;
        }
        
        if (topBound < parent->topBound)
        {
            topBound = parent->topBound;
            yStart = topBound - yPos;
        }

        if (bottomBound > parent->bottomBound)
        {
            bottomBound = parent->bottomBound;
        }
    }
}

void Window::refreshParent()
{
    if (parent != nullptr)
    {
        parent->refresh();
    }
}
