# LaserSeismograph
A laser hanging from a 1m long pendulum, functioning as a seismograph

# Known Bugs:
*The layout for the PCB wired the first photodiode, PD0, to the second input on the ADC, and the second photodiode PD1 is wired to the first input on the ADC. 
This is corrected in software.

* The pins for the ESP8266 Thing Dev do not fit in to the sockets as laid out: CS, EOC, MOSI need to be wired differently. 
CS is actually D16, EOC is D4, MOSI is D13. Those numbers are not the pin numbers on the dev board but their silkscreened numbers. 
