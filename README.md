# ArduinoIOSim
Simulator of Input/Output logical levels for Arduino

This is an ncursed UI simulator primarly for use with Arduino's, but can easily be adapted to any microcontroller. It allows to observe the behavior of the program logic, and interact with it, without using real hardware. The outputs are displayed in the console, and the inputs taken from the keyboard.

It is composed of a Simulator and a hardware abstraction layer, meaning that you will need to change your program so as to use this HAL lib, and write standard C++. You cannot use the Arduino library functions directly. 

Example:
In the Examples folder you will find the typical blink example as a reference. It blinks pin 13 and takes input from 10 and 11, mapped to Left and Right cursor keys.

To compile you must have ncurses installed in your system.

 g++ -DX86_PLAT -o blink main.cpp X86Platform.cpp -lncurses



