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

    int getWidth() const;
    int getHeight() const;

    void hide();
    void show();

    virtual void keyPressed (int key) = 0;

private:
    Window window;

    virtual void draw (Window &w) = 0;
    virtual void resized() = 0;
};

#endif // COMPONENT_HPP_INCLUDED
