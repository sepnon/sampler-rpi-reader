#include "SampleReader.h"

#include <SpiMessages.h>

using namespace std;

SampleReader::SampleReader(AvrSpiIo &avrSpi)
	: _avrSpi(avrSpi)
{
	_avrSpi.transceive(START_SAMPLING);
	_avrSpi.transceive(READ_SAMPLES);
}

SampleReader::~SampleReader()
{
	_avrSpi.transceive(NO_CMD);
	_avrSpi.transceive(STOP_SAMPLING);
}

vector<uint16_t> SampleReader::read()
{
	vector<uint16_t> samples;

	auto readSize = _avrSpi.transceive(READ_SAMPLES);
	if (readSize > 0)
	{
		samples.reserve(readSize);

		for (int i = 0; i < readSize; ++i)
		{
			uint8_t data[2];
			data[0] = _avrSpi.transceive(READ_SAMPLES);
			data[1] = _avrSpi.transceive(READ_SAMPLES);
			uint16_t sample = (static_cast<uint16_t>(data[0]) << 8) + data[1];
			samples.push_back(sample);
		}
	}
	return samples;
}
