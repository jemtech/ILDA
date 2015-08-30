# ILDA
This project is a DAC ([digital to analog converter](https://en.wikipedia.org/wiki/Digital-to-analog_converter)) which generates six [differential signals](https://en.wikipedia.org/wiki/Differential_signaling) based on the ILDA ([International Laser Display Association](http://www.laserist.org/)) [ISP-DB25 standard](http://www.laserist.org/StandardsDocs/ILDA_ISP99_rev002.pdf).
Two signals for the X and Y axis (-10V to +10V), four signals for the channels red, green and blue and one custom colour (0V to +5V).
The DAC is controlled via [SPI](https://en.wikipedia.org/wiki/Serial_Peripheral_Interface_Bus). In this case the software is written for a "Raspberry Pi", but any other controller can be used.

This project contains two sections:

1. Hardware: In this section you find a 16bit 6ch DAC for ILDA. Reaching 6.6kpps (measured, theoretical up to 400kpps) limited by the controling "Raspberry Pi". It is resulting in a resolution of 4294967296 positions (~4295 megapixels).
The asynchronous LDAC update pin at the DAC, immediately updates the DAC register with the contents of the input register, so all outputs change at the same time and can be synchronised with an external clock.
2. Software: C based software to control the ILDA-DAC with a "Raspberry Pi".

## Hardware
The hardware is designed with [KiCad](http://kicad-pcb.org/).
"ILDA/hardware/kicad/"
Using 5 channels (x,y,r,g,b) with 16bit at 50MHz SPI speed the DAC reaches theoretically 400kpps (50000KHz ÷ (5×24bit) = 416kpps).


## Software
### Dependencies
[Wiring Pi](http://wiringpi.com/).
### Installation
1. Install "Wiring Pi". Then follow the instructions on [Wiring Pi](http://wiringpi.com/).
2. Download this project to your "Raspberry Pi".
3. Change to the "ILDA/software/raspberrypi/" folder.
4. Run "make".

Now you can run the software with "sudo ./ildaNode".

