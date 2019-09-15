#ifndef LOCALAVRSPIIO_H
#define LOCALAVRSPIIO_H

#include <avr_io/AvrSpiIo.h>
#include <Spi.h>

class LocalAvrSpiIo : public AvrSpiIo
{
public:
	LocalAvrSpiIo(Spi spi);
	~LocalAvrSpiIo() = default;

	uint8_t transceive(uint8_t byte) override final;

private:
	Spi _spi;
}

#endif // LOCALAVRSPIIO_H
