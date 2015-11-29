#include "Component.hpp"

Component::Component()
    : window (Curses::createWindow (0, 0, 0, 0)),
      width (0), height (0)
{
}

Component::~Component()
{
}

void Component::setBounds (int newX, int newY, int newWidth, int newHeight)
{
    window = Curses::createWindow (newHeight, newWidth, newY, newX);
    width = newWidth;
    height = newHeight;

    redraw();
}

void Component::redraw()
{
    draw (window.get());
}

void Component::giveFocus()
{
    wrefresh (window.get());
}
