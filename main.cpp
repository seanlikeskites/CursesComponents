#include "Slider.hpp"

int main()
{
    Curses::Instance curses = Curses::getInstance();
    curses.setCursor (Curses::Cursor::none);

    Window::Pointer testWin = curses.createTopLevelWindow (30, 2, 30, 30);

    testWin->fillWithColour (Curses::Colour::white);

    testWin->setForegroundColour (Curses::Colour::green);

    for (int i = 0; i < 30; ++i)
    {
        testWin->drawLine (0, i, i, 29);
    }

    testWin->setForegroundColour (Curses::Colour::red);

    for (int i = 0; i < 30; ++i)
    {
        testWin->drawLine (i, 0, 29, i);
    }

    testWin->setForegroundColour (Curses::Colour::cyan);
    testWin->drawEllipse (4, 9, 22, 12, ACS_BLOCK);

    testWin->setForegroundColour (Curses::Colour::magenta);
    testWin->drawEllipse (5, 10, 20, 10, ACS_BLOCK);

    testWin->setForegroundColour (Curses::Colour::blue);
    testWin->drawEllipse (6, 11, 18, 8, ACS_BLOCK);

    testWin->setForegroundColour (Curses::Colour::yellow);
    testWin->drawEllipse (7, 12, 16, 6, ACS_BLOCK);

    testWin->setForegroundColour (Curses::Colour::black);
    testWin->drawEllipse (8, 13, 14, 4, ACS_BLOCK);

    Window::Pointer win2 = curses.createTopLevelWindow (27, 5, 10, 5);
    win2->fillWithColour (Curses::Colour::blue);

    Window::Pointer child2;

    {
        int childX = 2;
        int childY = 2;
        Window::Pointer testChild = testWin->createChildWindow (childX, childY, 35, 5);
        testChild->fillWithColour (Curses::Colour::magenta);
        testChild->setForegroundColour (Curses::Colour::yellow);
        testChild->drawLine (0, 0, 34, 4, ACS_BLOCK);
        
        testWin->refresh();

        const int numSliders = 4;
        Slider sliders [numSliders] = {{"Ham"}, {"Jam"}, {"Hats"}, {"Cake"}};

        int sliderWidth = 5;
        int sliderHeights [numSliders] = {30, 26, 35, 19};
        int sliderY = 0;
        int sliderX = 2;

        double sliderBottoms [numSliders] = {-4.0, 50.0, 37.6, 112.0};
        double sliderTops [numSliders] = {5.0, -12.0, 34.2, 2000.34};
        double sliderSkews [numSliders] = {1.0, 0.9, 12, 0.1};

        for (int s = 0; s < numSliders; ++s)
        {
            sliders [s].setBounds (sliderX, sliderY, sliderWidth, sliderHeights [s]);
            sliders [s].setRange (sliderBottoms [s], sliderTops [s], sliderSkews [s]);
            sliderX += sliderWidth;
        }

        int key;
        int sliderIndex = 0;

        child2 = testWin->createChildWindow (0, 0, 10, 10);
        child2->fillWithColour (Curses::Colour::yellow);

        curses.refreshScreen();

        while ((key = curses.getInputCharacter()) != '\n')
        {
            switch (key)
            {
                case KEY_RIGHT:
                    sliderIndex++;
                    sliderIndex %= numSliders;
                    break;

                case KEY_LEFT:
                    sliderIndex--;
                    sliderIndex = (sliderIndex + numSliders) % numSliders;
                    break;

                case 'h':
                    testWin->hide();
                    break;

                case 'r':
                    testWin->show();
                    break;

                case 'w':
                    testChild->move (childX, --childY);
                    break;
                    
                case 'a':
                    testChild->move (--childX, childY);
                    break;

                case 's':
                    testChild->move (childX, ++childY);
                    break;

                case 'd':
                    testChild->move (++childX, childY);
                    break;

                default:
                    sliders [sliderIndex].keyPressed (key);
                    break;
            }

            curses.refreshScreen();
        }
    }

    curses.refreshScreen();

    int key;

    while ((key = curses.getInputCharacter()) != '\n')
    {
        if (key == 'r')
        {
            child2->refresh();
        }
        else if (key == 'h')
        {
            child2->hide();
        }
        else if (key == 's')
        {
            child2->show();
        }

        curses.refreshScreen();
    }

    return 0;
}
