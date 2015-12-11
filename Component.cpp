#include "Component.hpp"

Component::Component()
    : window (Curses::getInstance().createWindow (0, 0, 0, 0))
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

    resized();
    redraw();
}

int Component::getWidth()
{
    return window.getWidth();
}

int Component::getHeight()
{
    return window.getHeight();
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
