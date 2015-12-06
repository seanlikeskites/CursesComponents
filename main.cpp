#include "Slider.hpp"

int main()
{
    Slider slider ("Test");
    slider.setBounds (2, 2, 6, 40);
    slider.setRange (2, 80, 0.2);

    int key;

    while ((key = getch()) != '\n')
    {
        slider.keyPressed (key);
    }

    return 0;
}
