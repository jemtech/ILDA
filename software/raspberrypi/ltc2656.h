
#if !defined(_LTC2656_H)
#define _LTC2656_H 1

static const int LDAC_PIN = 7;
static const int SPI_DEV = 0;
static const char *SPIDEV_1_PATH = "/dev/spidev0.1";
static const char *SPIDEV_0_PATH = "/dev/spidev0.0";

void setChVal(unsigned char channel, unsigned char valUpper, unsigned char valLower);
void executeValues();
void initLtc2656();
void closeLtc2656();

#endif
