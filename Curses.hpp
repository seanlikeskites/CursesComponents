#ifndef CURSES_HPP_INCLUDED
#define CURSES_HPP_INCLUDED

#include <memory>
#include <string>
#include <mutex>
#include <vector>
#include <curses.h>

class Window;

/** A singleton class which manages the lifetime of the ncurses library. */
class Curses
{
public:
    using PadPointer = std::unique_ptr <WINDOW, int(*)(WINDOW*)>;
    using Instance = Curses&;

    /** Destructor */
    ~Curses();

    /** Get the singleton instance of the ncurses library. */
    static Instance getInstance();

    int getInputCharacter();

    /** Create a new window. 
     *
     *  @param x the x position of the new window
     *  @param y the y position of the new window
     *  @param width the width of the new window
     *  @param height the height of the new window
     */
    std::shared_ptr <Window> createTopLevelWindow (int x, int y, int width, int height);

    /** Returns the width of the terminal in characters. */
    int getScreenWidth() const;
    /** Returns the height of the terminal in characters. */
    int getScreenHeight() const;

    /** An enum type for the standard ncurses colours. */
    enum class Colour : short
    {
        black = COLOR_BLACK, /**< Black */
        red = COLOR_RED, /**< Red */
        green = COLOR_GREEN, /**< Green */
        yellow = COLOR_YELLOW, /**< Yellow */
        blue = COLOR_BLUE, /**< Blue */
        magenta = COLOR_MAGENTA, /**< Magenta */
        cyan = COLOR_CYAN, /**< Cyan */
        white = COLOR_WHITE /**< White */
    };

    using ColourPair = short;

    /** Return the index for a given pair of colours.
     *
     *  When the Curses singleton is first instantiated every possible pair of colours is
     *  initialised to a given index. This function returns the index a particular pair
     *  was initialised to.
     *
     *  @param backgroundColour the background colour of the pair
     *  @param foregroundColour the foreground colour of the pair
     */
    static ColourPair getColourPairIndex (Colour backgroundColour, Colour foregroundColour);

    /** An enum type for cursor types. */
    enum class Cursor : int
    {
        none, /** No cursor */
        visible, /** Underscore type cursor */
        highlyVisible /** Block type cursor */
    };

    /** Set the cursor type.
     *  
     *  @param newCursor the cursor type to use
     */
    void setCursor (Cursor newCursor);

    /** Refresh the screens contents. */
    void refreshScreen();

    /** A class to protect calls to ncurses functions. */
    class Lock
    {
    public:
        /** Contructor */
        Lock();
        /** Destructor */
        ~Lock();

    private:
        std::unique_lock <std::recursive_mutex> lock;
    };

private:
    Curses();
    Curses (const Curses&) = delete;
    Curses (Curses&&) = delete;
    Curses& operator= (const Curses&) = delete;
    Curses& operator= (Curses&&) = delete;

    static std::recursive_mutex protectionMutex;

    std::unique_ptr <Window> mainWindow;
};

/** An ncurses window. */
class Window
{
public:
    using Pointer = std::shared_ptr <Window>;

    /** Destructor */
    ~Window();

    Pointer createChildWindow (int x, int y, int width, int height);

    void refresh ();
    /** Hide the window. */
    void hide();
    /** Show the window. */
    void show();
    /** Move the window to a new position. 
     *
     *  @param x the new x position
     *  @param y the new y position
     */
    void move (int x, int y);
    /** Resize the window.
     *
     *  @param x the new x position
     *  @param y the new y position
     *  @param newWidth the new width
     *  @param newHeight the new height
     */
    void resize (int x, int y, int newWidth, int newHeight);

    /** Print a character at the current cursor position.
     *
     *  @param character the character to print
     */
    void printCharacter (const chtype character);
    /** Print a character at the given position.
     *
     *  @param character the character to print
     *  @param x the x position of the character
     *  @param y the y position of the character
     */
    void printCharacter (const chtype character, int x, int y);

    /** Print a string at the current cursor position.
     *
     *  @param string the string to print
     */
    void printString (const std::string &string);
    /** Print a string at the given position.
     *
     *  @param string the string to print
     *  @param x the x position of the string
     *  @param y the y position of the string
     */
    void printString (const std::string &string, int x, int y);

    /** Print a floating point number at the current cursor position.
     *
     *  @param value the value to print
     */
    void printDouble (double value);
    /** Print a floating point number at the given position.
     *
     *  @param value the value to print
     *  @param x the x position of the value
     *  @param y the y position of the value
     */
    void printDouble (double value, int x, int y);

    /** Print an integer at the current cursor position.
     *
     *  @param value the value to print
     */
    void printInteger (int value);
    /** Print an integer at the given position.
     *
     *  @param value the value to print
     *  @param x the x position of the value
     *  @param y the y position of the value
     */
    void printInteger (int value, int x, int y);

    /** Draw a straight line.
     *
     *  @param startX the x position of the start of the line
     *  @param startY the y position of the start of the line
     *  @param endX the x position of the end of the line
     *  @param endY the y position of the end of the line
     *  @param character the character to use for printing the line
     */
    void drawLine (int startX, int startY, int endX, int endY, const chtype character = ACS_BLOCK);
    /** Draw an ellipse.
     *
     *  @param x the x position
     *  @param y the y position
     *  @param width the width of the ellipse
     *  @param height the height of the ellipse
     *  @param character the character to use for printing the ellipse
     */
    void drawEllipse (int x, int y, int width, int height, const chtype character = '.');

    /** Draw a box.
     *
     *  @param x the x position
     *  @param y the y position
     *  @param width the width of the box
     *  @param height the height of the box
     */
    void drawBox (int x, int y, int width, int height);

    /** Fill the entire window with a character.
     *  
     *  @param character the character to print
     */
    void fillAll(const chtype character);
    /** Clear the window. */
    void clear();

    /** Returns the windows width. */
    int getWidth() const;
    /** Returns the windows height. */
    int getHeight() const;

    /** A structure which holds ncurses video attributes. */
    struct VideoAttributes
    {
        attr_t attributes; /**< The attributes. */
        short colourPair; /**< The colour pair. */
    };

    /** Returns the currently set video attributes for this window. */
    VideoAttributes getVideoAttributes() const;
    /** Sets the video attributes for this window. 
     *
     *  The new attributes will be applied to any drawing done on the window after this call.
     *
     *  @param attributes the new attributes to set.
     */
    void setVideoAttributes (const VideoAttributes &attributes);
    /** Set the background colour for this window.
     *
     *  The new colour will be applied to any drawing done on the window after this call.
     *
     *  @param newBackgroundColour the new background colour
     */
    void setBackgroundColour (Curses::Colour newBackgroundColour);
    /** Set the foreground colour for this window.
     *
     *  The new colour will be applied to any drawing done on the window after this call.
     *
     *  @param newForegroundColour the new foreground colour
     */
    void setForegroundColour (Curses::Colour newForegroundColour);
    /** Set the background and foreground colours for this window.
     *
     *  The new colours will be applied to any drawing done on the window after this call.
     *
     *  @param newBackgroundColour the new background colour
     *  @param newForegroundColour the new foreground colour
     */
    void setColours (Curses::Colour newBackgroundColour, Curses::Colour newForegroundColour);

    /** Set the bold attribute for this window.
     *  @param setting the setting of the bold attribute
     */
    void setBold (bool setting);
    /** Set the underline attribute for this window.
     *  @param setting the setting of the underline attribute
     */
    void setUnderline (bool setting);

    int getCharacter();

private:
    Window (int x, int y, int widthInit, int heightInit, 
            Window *parentInit, Window *nextSiblingInit);
    Window (Window &other) = delete;
    Window& operator= (Window &rhs) = delete;

    int xPos, yPos;
    int width, height;

    bool visible;

    Curses::PadPointer window;
    Curses::PadPointer blankWindow;

    Curses::Colour backgroundColour, foregroundColour;

    Window *parent;
    Window *nextSibling;
    Window *bottomChild, *topChild;

    friend class Curses;
};

#endif // CURSES_HPP_INCLUDED
