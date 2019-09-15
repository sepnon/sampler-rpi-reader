#ifndef AVROSCILLOSCOPE_H
#define AVROSCILLOSCOPE_H

#include <avr_io/AvrSpiIo.h>
#include "SampleReader.h"
#include "SpiProfiler.h"

class AvrOscilloscope
{
public:
	AvrOscilloscope();

	SampleReader startSampling();

	SpiProfiler profileSpi();

	uint8_t readSamplingState();

	void incByte();
	uint8_t readByte();

private:
	void zeroBomb(unsigned count);
};

#endif // AVROSCILLOSCOPE_H
