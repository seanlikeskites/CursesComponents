#include "Slider.hpp"

int main()
{
    Curses curses;

    Slider slider ("Test Slider");
    slider.setBounds (20, 2, 10, 10);
    slider.giveFocus();


    return 0;
}
