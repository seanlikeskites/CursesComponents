#ifndef COMPONENT_HPP_INCLUDED
#define COMPONENT_HPP_INCLUDED

#include "Curses.hpp"

class Component
{
public:
    Component();
    virtual ~Component();

    void redraw();

    void setBounds (int newX, int newY, int newWidth, int newHeight);

    int getWidth();
    int getHeight();

    void hide();
    void show();

private:
    Window window;

    int width, height;

    virtual void draw (Window &w) = 0;
    virtual void resized() = 0;
};

#endif // COMPONENT_HPP_INCLUDED
