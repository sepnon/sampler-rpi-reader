#ifndef SAMPLEREADER_H
#define SAMPLEREADER_H

#include <avr_io/Spi.h>
#include <vector>

class SampleReader
{
	public:
		SampleReader(Spi &avrSpi);
		~SampleReader();

		std::vector<uint16_t> read();

	private:
		std::vector<uint16_t> read(uint8_t byteToSend);
		
		Spi &_avrSpi;
};

#endif // SAMPLEREADER_H
