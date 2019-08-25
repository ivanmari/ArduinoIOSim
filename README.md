# ArduinoIOSim
Simulator of Input/Output logical levels for Arduino

This is an ncursed UI simulator that is primarly for use with Arduino's, but can easily be adapted to any microcontroller. It allows to observe the behavior of the program logic, and interact with it, without using real hardware. The outputs are displayed in the console, and the inputs taken from the keyboard. The keyboard persistence, 100ms by default, is configurable by a preprocessor constant in ui.cpp file.

This simulator is based on a HAL (Hardware Abstraction Layer), that makes the code independent of the underlying MCU architecture. A compilation flag passed to the compiler command line, will allow switching between platforms in a transparent way. This has the advantage that your code could be easily ported to another MCUs other than Arduinos, but it will force you to change your program so as to use this HAL lib, and write standard C++. You cannot use the Arduino library functions directly.  

Example:
In the Examples folder you will find the typical blink example as a reference. It blinks pin 13 and takes input from 10 and 11, mapped to Left and Right cursor keys.

To compile you must have ncurses installed in your system.

g++ -g -DX86_PLAT -o blink main.cpp X86Platform.cpp ui.cpp -lncurses -lpthread


