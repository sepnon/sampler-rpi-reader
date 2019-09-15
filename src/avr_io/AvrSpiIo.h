#ifndef AVRSPIIO_H
#define AVRSPIIO_H

#include <cstdint>

class AvrSpiIo
{
public:
	virtual ~AvrSpiIo() = default;

	virtual uint8_t transceive(uint8_t byte) = 0;
}

#endif // AVRSPIIO_H
