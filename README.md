# LaserSeismograph
A laser hanging from a 1m long pendulum, functioning as a seismograph

# Contents
serial_print.pde is the Processing script used to take in serial values and save in a .csv file. 
PHOTODIODE_BOARD.ino is the arduino script which runs on the ESP8266.
The schematic and two layout files are also included. 

# Known Bugs:
*The layout for the PCB wired the first photodiode, PD0, to the second input on the ADC, and the second photodiode PD1 is wired to the first input on the ADC. 
This is corrected in software.

* The pins for the ESP8266 Thing Dev do not fit in to the sockets as laid out: CS, EOC, MOSI need to be wired differently. 
CS is actually D16, EOC is D4, MOSI is D13. Those numbers are not the pin numbers on the dev board but their silkscreened numbers. 
