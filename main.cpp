#include "Slider.hpp"

int main()
{
    Slider slider ("Test");
    slider.setBounds (2, 2, 6, 40);

    getch();

    slider.setValue (0.5);

    getch();

    slider.setValue (5);

    getch();

    slider.setValue (-5);

    getch();

    slider.setValue (0.25);

    getch();

    return 0;
}
