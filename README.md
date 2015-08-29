# ILDA
This project contains 2 sections:

1. hardware: in this section you find a 16bit 6ch DAC for ILDA. Reaching 6.6kpps (measured, theoretical up to 400kpps) limited by the controlling "Raspberry Pi". Resulting in a resulution of 4294967296 positions (~4295 megapixels).
2. software: c based software to control the ILDA-DAC with a "Raspberry Pi".

## Hardware
The hardware is designed with [KiCad](http://kicad-pcb.org/).
"ILDA/hardware/kicad/"
Using 5 channals (x,y,r,g,b) with 16bit at 50MHz SPI speed the DAC reaches theoretically 400kpps. (50000KHz ÷ (5×24bit) = 416kpps)


## Software
### Dependencies
[Wiring Pi](http://wiringpi.com/).
### Installation
1. Intall "Wiring Pi" follow the instruction on [Wiring Pi](http://wiringpi.com/).
2. Download this project to your "Raspberry Pi".
3. Change to the "ILDA/software/raspberrypi/" folder.
4. Run "make".

Now you can run the software with "sudo ./ildaNode".

