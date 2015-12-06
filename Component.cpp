#include "Component.hpp"

Component::Component()
    : window (Curses::getInstance().createWindow (0, 0, 0, 0)),
      width (0), height (0)
{
}

Component::~Component()
{
}

void Component::redraw()
{
    window.clear();
    Window::VideoAttributes attributeCache = window.getVideoAttributes();
    draw (window);
    window.setVideoAttributes (attributeCache);
    update_panels();
    doupdate();
}

void Component::setBounds (int newX, int newY, int newWidth, int newHeight)
{
    window.resize (newX, newY, newWidth, newHeight);

    width = newWidth;
    height = newHeight;

    resized();
    redraw();
}

int Component::getWidth()
{
    return width;
}

int Component::getHeight()
{
    return height;
}

void Component::hide()
{
    window.hide();
}

void Component::show()
{
    window.show();
    redraw();
}
