#ifndef SAMPLEREADER_H
#define SAMPLEREADER_H

#include <avr_io/AvrSpiIo.h>
#include <vector>

class SampleReader
{
	public:
		SampleReader(AvrSpiIo &avrSpi);
		~SampleReader();

		std::vector<uint16_t> read();

	private:
		AvrSpiIo &_avrSpi;
};

#endif // SAMPLEREADER_H
