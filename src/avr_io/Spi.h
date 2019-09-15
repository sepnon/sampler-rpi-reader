#ifndef SPI_H
#define SPI_H

#include <cstdint>

class Spi
{
public:
	Spi(int mode = 0, int channel = 0);

	uint8_t transceive(uint8_t byteToWrite);
	
	Spi(const Spi &) = delete;
	Spi &operator=(const Spi &) = delete;
	Spi(Spi &&) noexcept = default;
	Spi &operator=(Spi &&) noexcept = default;

private:
	void writeSpiMode(char mode);
	char readSpiMode();

	int _channel;
	int _fd;
}

#endif // SPI_H
