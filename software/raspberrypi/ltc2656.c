#include <stdio.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <fcntl.h>		//Needed for SPI port
#include <sys/ioctl.h>		//Needed for SPI port
#include <linux/spi/spidev.h>	//Needed for SPI port
#include <unistd.h>		//Needed for SPI port (close)
#include "ltc2656.h"

int spi_cs0_fd;			//file descriptor for the SPI device
int spi_cs1_fd;			//file descriptor for the SPI device


//SET SPI MODE
unsigned char SPI_MODE = SPI_MODE_0; //SPI_MODE_0 (0,0) CPOL = 0, CPHA = 0, Clock idle low, data is clocked in on rising edge, output data (change) on falling edge


//***********************************
//***********************************
//********** SPI OPEN PORT **********
//***********************************
//***********************************
//spi_device	0=CS0, 1=CS1
int SpiOpenPort (int spi_device)
{
    int status_value = -1;
    int *spi_cs_fd;
    
    if (spi_device)
    	spi_cs_fd = &spi_cs1_fd;
    else
    	spi_cs_fd = &spi_cs0_fd;


    if (spi_device)
    	*spi_cs_fd = open(SPIDEV_1_PATH, O_RDWR);
    else
    	*spi_cs_fd = open(SPIDEV_0_PATH, O_RDWR);

    if (*spi_cs_fd < 0)
    {
        perror("Error - Could not open SPI device");
        exit(1);
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_MODE, &SPI_MODE);
    if(status_value < 0)
    {
        perror("Could not set SPIMode (WR)...ioctl fail");
        exit(1);
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_MODE, &SPI_MODE);
    if(status_value < 0)
    {
      perror("Could not set SPIMode (RD)...ioctl fail");
      exit(1);
    }

	unsigned char bitsPerWord = SPI_BITS_PER_WORD;
    status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_BITS_PER_WORD, &bitsPerWord);
    if(status_value < 0)
    {
      perror("Could not set SPI bitsPerWord (WR)...ioctl fail");
      exit(1);
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_BITS_PER_WORD, &bitsPerWord);
    if(status_value < 0)
    {
      perror("Could not set SPI bitsPerWord(RD)...ioctl fail");
      exit(1);
    }
	unsigned int spiSpeed = SPI_SPEED;
    status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spiSpeed);
    if(status_value < 0)
    {
      perror("Could not set SPI speed (WR)...ioctl fail");
      exit(1);
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_MAX_SPEED_HZ, &spiSpeed);
    if(status_value < 0)
    {
      perror("Could not set SPI speed (RD)...ioctl fail");
      exit(1);
    }
    return(status_value);
}

//************************************
//************************************
//********** SPI CLOSE PORT **********
//************************************
//************************************
int SpiClosePort (int spi_device)
{
	int status_value = -1;
    int *spi_cs_fd;

    if (spi_device)
    	spi_cs_fd = &spi_cs1_fd;
    else
    	spi_cs_fd = &spi_cs0_fd;


    status_value = close(*spi_cs_fd);
    if(status_value < 0)
    {
    	perror("Error - Could not close SPI device");
    	exit(1);
    }
    return(status_value);
}

//*******************************************
//*******************************************
//********** SPI WRITE & READ DATA **********
//*******************************************
//*******************************************
//data		Bytes to write.  Contents is overwritten with bytes read.
int SpiWriteAndRead (int spi_device, unsigned char *data, int length){
	struct spi_ioc_transfer spi[length];
	int i = 0;
	int retVal = -1;
	int *spi_cs_fd;

    if (spi_device)
    	spi_cs_fd = &spi_cs1_fd;
    else
    	spi_cs_fd = &spi_cs0_fd;

	//one spi transfer for each byte

	for (i = 0 ; i < length ; i++)
	{
		spi[i].tx_buf        = (unsigned long)(data + i); // transmit from "data"
		spi[i].rx_buf        = (unsigned long)(data + i) ; // receive into "data"
		spi[i].len           = sizeof(*(data + i)) ;
		spi[i].delay_usecs   = 0 ;
		spi[i].speed_hz      = SPI_SPEED ;
		spi[i].bits_per_word = SPI_BITS_PER_WORD ;
		spi[i].cs_change = 0;
		spi[i].pad = 0;
	}
	retVal = ioctl(*spi_cs_fd, SPI_IOC_MESSAGE(length), &spi) ;
	if(retVal < 0)
	{
		perror("Error - Problem transmitting spi data..ioctl");
		exit(1);
	}

	return retVal;
}

void setChVal(unsigned char channel, unsigned char valUpper, unsigned char valLower){
	unsigned char data[] = {channel, valUpper, valLower};
	SpiWriteAndRead(0, data, 3);
}

void setChVal_int(unsigned char channel, unsigned int val){
	setChVal(channel, (unsigned char)(val>>8), val & 0xff);
}

void setChVal_float(unsigned char channel, float val){
	setChVal_int(channel, val * 32768 + 32767);
}

void executeValues(){
	digitalWrite (LDAC_PIN, LOW);
	delayMicroseconds(1);
	digitalWrite (LDAC_PIN,  HIGH);
}

void initLtc2656(){
	wiringPiSetup ();
	pinMode (LDAC_PIN, OUTPUT);
	digitalWrite (LDAC_PIN,  HIGH);
	SpiOpenPort(SPI_DEV);
}

void closeLtc2656(){
	SpiClosePort(SPI_DEV);
}


