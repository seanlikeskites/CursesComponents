#ifndef CURSES_HPP_INCLUDED
#define CURSES_HPP_INCLUDED

#include <memory>
#include <curses.h>
#include <panel.h>

class Window;

class Curses
{
public:
    using WindowPointer = std::unique_ptr <WINDOW, int(*)(WINDOW*)>;
    using PanelPointer = std::unique_ptr <PANEL, int(*)(PANEL*)>;
    using Instance = Curses&;

    ~Curses();

    static Curses& getInstance();

    Window createWindow (int x, int y, int width, int height);

    int getScreenWidth();
    int getScreenHeight();

private:
    Curses();
    Curses (const Curses&) = delete;
    Curses (Curses&&) = delete;
    Curses& operator= (const Curses&) = delete;
    Curses& operator= (Curses&&) = delete;
};

class Window
{
public:
    Window (Window &&other);
    Window& operator= (Window &&rhs);
    ~Window();

    void hide();
    void show();
    void move (int x, int y);
    void resize (int x, int y, int width, int height);

    void addCharacter (const chtype character);
    void addCharacter (const chtype character, int x, int y);

    void addString (const char *string);
    void addString (const char *string, int x, int y);

    void waitForInput();

private:
    Window (int x, int y, int width, int height);

    Curses::WindowPointer window;
    Curses::PanelPointer panel;

    friend class Curses;
};

#endif // CURSES_HPP_INCLUDED
