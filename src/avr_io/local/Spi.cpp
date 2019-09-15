#include "driver.h"

#include <iostream>

#include <wiringPiSPI.h>

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

using namespace std;

Spi::Spi(int mode, int channel)
	: _channel(channel)
{
	cout << "Initializing" << endl ;
	_fd = wiringPiSPISetup(_channel, 2'000'000);
	cout << "File descriptor: " << _fd << endl;

	writeSpiMode(mode);
	cout << "Spi mode: " << int(readSpiMode()) << endl;
}

void Spi::writeSpiMode(char mode)
{
	ioctl(_fd, SPI_IOC_WR_MODE, &mode);
}

char Spi::readSpiMode()
{
	char mode = 0;
	ioctl(_fd, SPI_IOC_RD_MODE, &mode);
	return mode;
}

uint8_t Spi::transceive(uint8_t writeData)
{
	wiringPiSPIDataRW(_channel, &writeData, 1);
	return writeData;
}

