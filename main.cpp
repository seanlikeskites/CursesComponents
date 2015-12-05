#include "Slider.hpp"

int main()
{
    Slider slider;
    slider.setBounds (12, 1, 10, 10);

    Slider slider2;
    slider2.setBounds (30, 15, 9, 5);

    getch();

    slider2.hide();

    getch();

    slider.hide();

    getch();

    slider.show();
    slider2.show();
    update_panels();
    doupdate();

    getch();

    slider2.setBounds (3, 4, 2, 2);

    getch();

    return 0;
}
