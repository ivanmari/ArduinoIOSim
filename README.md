# ArduinoIOSim
Simulator of Input/Output logical levels for Arduino

This is an ncursed UI simulator primarly for use with Arduino's, but can easily be adapted to any microcontroller. It allows to observe the behavior of the program logic, and interact with it, without using real hardware. The outputs are displayed in the console, and the inputs taken from the keyboard.
It is composed of a Simulator and a hardware abstraction layer, meaning that you will need to change your program so as to use this HAL lib, and write standard C++. You cannot use the Arduino library functions directly. In the Examples folder you will find the typical blink example as a reference. 


