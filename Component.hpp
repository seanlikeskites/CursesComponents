#ifndef COMPONENT_HPP_INCLUDED
#define COMPONENT_HPP_INCLUDED

#include "CursesHelpers.hpp"

class Component
{
public:
    Component();
    virtual ~Component();

    void setBounds (int newX, int newY, int newWidth, int newHeight);

    void redraw();

    void giveFocus();

private:
    Curses::WindowPointer window;

    int height, width;

    virtual void draw (WINDOW *w) = 0;
};

#endif // COMPONENT_HPP_INCLUDED
