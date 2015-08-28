
#if !defined(_LTC2656_H)
#define _LTC2656_H 1
//GPOI pin for LDAC in wireingPi
static const int LDAC_PIN = 7; // = P4
//spi device 0=CS0, 1=CS1
static const int SPI_DEV = 0;
//SPI BUS SPEED
static const unsigned int SPI_SPEED = 50000000;//= 50MHz
//SET BITS PER WORD
static const unsigned char SPI_BITS_PER_WORD = 8;
//SET SPI MODE
unsigned char SPI_MODE = SPI_MODE_0; //SPI_MODE_0 (0,0) CPOL = 0, CPHA = 0, Clock idle low, data is clocked in on rising edge, output data (change) on falling edge
static const char *SPIDEV_1_PATH = "/dev/spidev0.1";
static const char *SPIDEV_0_PATH = "/dev/spidev0.0";

void setChVal(unsigned char channel, unsigned char valUpper, unsigned char valLower);
void setChVal_int(unsigned char channel, unsigned int val);
void setChVal_float(unsigned char channel, float val);

void executeValues();
void initLtc2656();
void closeLtc2656();

#endif
