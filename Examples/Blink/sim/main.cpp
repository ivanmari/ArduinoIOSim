#include "../Blink.ino"
#include "ui.h"

int main()
{
    setup();

    startInputOutputSim();

    while(true)
    {
        loop();
    }
}
