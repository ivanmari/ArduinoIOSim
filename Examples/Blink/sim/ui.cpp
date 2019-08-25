#include <thread>
#include <mutex>
#include <ncurses.h>
#include <sstream>
#include <iomanip>
#include <chrono>

#define IS_INPUT 0x00
#define IS_OUTPUT 0x0F
#define IS_HIGH 0xF0
#define TOTAL_IO_COUNT 14
#define KEY_PERSISTENCE 100

#include "ui.h"

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::steady_clock;

std::mutex g_i_mutex;
unsigned char GpioSnapshot[TOTAL_IO_COUNT] = {};

void displayGpios();

void startInputOutputSim()
{
	std::thread gpio_reader(displayGpios);
	gpio_reader.detach();
}


void displayGpios()
{
    initscr();
    nodelay(stdscr, TRUE);
    noecho();
    cbreak();
    int row,col;
    getmaxyx(stdscr,row,col);

    col = (col - TOTAL_IO_COUNT*4)/2;

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
	
	bool display_loop = true;
    unsigned long key_pressed_timestamp = 0;
	unsigned long now = 0;
	
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
		now = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();

        switch(ch)
        {
        case 68:
            key_pressed = true;
            key_pressed_timestamp = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
            GpioSnapshot[10] &= 0x00;
            break;
        case 67:
            key_pressed = true;
            key_pressed_timestamp = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
            GpioSnapshot[11] &= 0x00;
            break;
        default:
            if(key_pressed_timestamp + KEY_PERSISTENCE > now)
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
