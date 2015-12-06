#ifndef CURSES_HPP_INCLUDED
#define CURSES_HPP_INCLUDED

#include <memory>
#include <unordered_set>
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

    enum class Colour : short
    {
        black = COLOR_BLACK,
        red = COLOR_RED,
        green = COLOR_GREEN,
        yellow = COLOR_YELLOW,
        blue = COLOR_BLUE,
        magenta = COLOR_MAGENTA,
        cyan = COLOR_CYAN,
        white = COLOR_WHITE
    };

    using ColourPair = short;

    ColourPair getColourPairIndex (Colour backgroundColour, Colour foregroundColour);

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

    void printCharacter (const chtype character);
    void printCharacter (const chtype character, int x, int y);

    void printString (const std::string &string);
    void printString (const std::string &string, int x, int y);

    void printDouble (double value);
    void printDouble (double value, int x, int y);

    void printInteger (int value);
    void printInteger (int value, int x, int y);

    struct VideoAttributes
    {
        attr_t attributes;
        short colourPair;
    };

    VideoAttributes getVideoAttributes() const;
    void setVideoAttributes (const VideoAttributes &attributes);
    void setBackgroundColour (Curses::Colour newBackgroundColour);
    void setForegroundColour (Curses::Colour newForegroundColour);
    void setColours (Curses::Colour newBackgroundColour, Curses::Colour newForegroundColour);

    void setBold (bool setting);
    void setUnderline (bool setting);

private:
    Window (int x, int y, int width, int height);

    Curses::WindowPointer window;
    Curses::PanelPointer panel;

    Curses::Colour backgroundColour, foregroundColour;

    friend class Curses;
};

#endif // CURSES_HPP_INCLUDED
