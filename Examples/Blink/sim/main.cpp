#include "../Blink.ino"
#include <thread>
#include <ncurses.h>
#include <sstream>
#include <iomanip>

#define IS_INPUT 0x00
#define IS_OUTPUT 0x0F
#define IS_HIGH 0xF0

unsigned char GpioSnapshot[14] = {};
bool display_loop = true;

void displayGpios()
{
    initscr();
    nodelay(stdscr, TRUE);
    noecho();
    cbreak();
    int row,col;
    getmaxyx(stdscr,row,col);

    col = (col - 25*4)/2;

    if(col < 0)
        col = 0;

    bool key_pressed = false;
    unsigned key_counter = 0;

    std::string pins;
    std::stringstream pins_stream(pins);

    for(unsigned i = 1; i <= sizeof(GpioSnapshot)/sizeof(unsigned char); ++i)
    {
        pins_stream << std::setw(2) << std::setfill('0') << i << "  ";
    }

    mvprintw(row/2 + 1, col, "%s", pins_stream.str().c_str());

    while(display_loop)
    {
        pins = "";
        for(unsigned i = 1; i <= sizeof(GpioSnapshot)/sizeof(unsigned char); ++i)
        {
            if((GpioSnapshot[i] & IS_OUTPUT))
            {
                if(GpioSnapshot[i] & IS_HIGH)
                {
                    pins += " X";
                    pins += "  ";
                }
                else
                {
                    pins += " _";
                    pins += "  ";
                }
            }
            else
            {
                if(GpioSnapshot[i])
                {
                    pins += " I";
                    pins += "  ";
                }
                else
                {
                    pins += " i";
                    pins += "  ";
                }
            }
        }

        mvprintw(row/2, col, "%s", pins.c_str());
        refresh();

        char ch = getch();

        switch(ch)
        {
        case 68:
            key_pressed = true;
            key_counter = 250;
            GpioSnapshot[10] &= 0x00;
            break;
        case 67:
            key_pressed = true;
            key_counter = 250;
            GpioSnapshot[11] &= 0x00;
            break;
        default:
            if(key_pressed = true && key_counter--)
            {
                break;
            }
            else
            {
                key_pressed = false;
                GpioSnapshot[10] |= 0xF0;
                GpioSnapshot[11] |= 0xF0;
            }

        }
    }
}

int main()
{
    setup();

    std::thread gpio_reader(displayGpios);

    while(true)
    {
        loop();
    }
}
