#include "AvrOscilloscope.h"
#include "SpiMessages.h"

AvrOscilloscope::AvrOscilloscope()
{
	zeroBomb(32);
}

uint8_t AvrOscilloscope::readSamplingState()
{
	transceive(READ_STATUS);
	return transceive(NO_CMD);
}

void AvrOscilloscope::incByte() { transceive(INC_BYTE); }
uint8_t AvrOscilloscope::readByte()
{
	transceive(READ_BYTE);
	return transceive(NO_CMD);
}

void AvrOscilloscope::zeroBomb(unsigned count)
{
	while (count-- > 0)
	{
		cout << int(transceive(0)) << "; ";
	}
}

