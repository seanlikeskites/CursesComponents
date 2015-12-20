#ifndef CURSES_HPP_INCLUDED
#define CURSES_HPP_INCLUDED

#include <memory>
#include <string>
#include <curses.h>
#include <panel.h>

class Window;

/** A singleton class which manages the lifetime of the ncurses library. */
class Curses
{
public:
    using WindowPointer = std::unique_ptr <WINDOW, int(*)(WINDOW*)>;
    using PanelPointer = std::unique_ptr <PANEL, int(*)(PANEL*)>;
    using Instance = Curses&;

    /** Destructor */
    ~Curses();

    /** Get the singleton instance of the ncurses library. */
    static Curses& getInstance();

    /** Create a new window. 
     *
     *  @param x the x position of the new window
     *  @param y the y position of the new window
     *  @param width the width of the new window
     *  @param height the height of the new window
     */
    Window createWindow (int x, int y, int width, int height);

    /** Returns the width of the terminal in characters. */
    int getScreenWidth() const;
    /** Returns the height of the terminal in characters. */
    int getScreenHeight() const;

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

/** An ncurses panel. */
class Window
{
public:
    Window (Window &&other);
    Window& operator= (Window &&rhs);
    ~Window();

    void hide();
    void show();
    void move (int x, int y);
    void resize (int x, int y, int newWidth, int newHeight);

    void printCharacter (const chtype character);
    void printCharacter (const chtype character, int x, int y);

    void printString (const std::string &string);
    void printString (const std::string &string, int x, int y);

    void printDouble (double value);
    void printDouble (double value, int x, int y);

    void printInteger (int value);
    void printInteger (int value, int x, int y);

    void drawLine (int staryX, int startY, int endX, int endY, const chtype character = ACS_BLOCK);
    void drawEllipse (int x, int y, int width, int height, const chtype character = '.');

    void fillAll(const chtype character);
    void clear();

    int getWidth() const;
    int getHeight() const;

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
    Window (int x, int y, int widthInit, int heightInit);

    int width, height;

    Curses::WindowPointer window;
    Curses::PanelPointer panel;

    Curses::Colour backgroundColour, foregroundColour;

    friend class Curses;
};

#endif // CURSES_HPP_INCLUDED
