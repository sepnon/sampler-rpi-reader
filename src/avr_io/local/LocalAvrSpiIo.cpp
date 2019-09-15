#include "LocalAvrSpiIo.h"

LocalAvrSpiIo::LocalAvrSpiIo(Spi spi)
	: _spi(std::move(spi))
{
}

uint8_t LocalAvrSpiIo::transceive(uint8_t byte)
{
	return _spi.transceive(byte);
}
