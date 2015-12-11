#include "Slider.hpp"

int main()
{
    Window testWin = Curses::getInstance().createWindow (30, 2, 30, 30);
    testWin.setForegroundColour (Curses::Colour::green);

    for (int i = 0; i < 30; ++i)
    {
        testWin.drawLine (0, i, i, 29, '.');
    }

    testWin.setForegroundColour (Curses::Colour::red);

    for (int i = 0; i < 30; ++i)
    {
        testWin.drawLine (i, 0, 29, i, '.');
    }

    testWin.setForegroundColour (Curses::Colour::white);
    testWin.printString ("Hello World!", 9, 15);

    testWin.setForegroundColour (Curses::Colour::cyan);
    testWin.drawEllipse (4, 9, 22, 12);

    testWin.setForegroundColour (Curses::Colour::magenta);
    testWin.drawEllipse (5, 10, 20, 10);

    testWin.setForegroundColour (Curses::Colour::blue);
    testWin.drawEllipse (6, 11, 18, 8);

    testWin.setForegroundColour (Curses::Colour::yellow);
    testWin.drawEllipse (7, 12, 16, 6);

    const int numSliders = 4;
    Slider sliders [numSliders] = {{"Ham"}, {"Jam"}, {"Hats"}, {"Cake"}};

    int sliderWidth = 5;
    int sliderHeights [numSliders] = {30, 26, 35, 19};
    int sliderY = 2;
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

    while ((key = getch()) != '\n')
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

            default:
                sliders [sliderIndex].keyPressed (key);
                break;
        }
    }


    return 0;
}
