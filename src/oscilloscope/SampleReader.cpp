#include "SampleReader.h"

#include "SpiMessages.h"

using namespace std;

SampleReader::SampleReader(Spi &avrSpi)
	: _avrSpi(avrSpi)
{
	_avrSpi.transceive(START_SAMPLING);
	_avrSpi.transceive(READ_SAMPLES);
}

SampleReader::~SampleReader()
{
	read(NO_CMD);
	_avrSpi.transceive(STOP_SAMPLING);
}

vector<uint16_t> SampleReader::read()
{
	return read(READ_SAMPLES);
}

vector<uint16_t> SampleReader::read(uint8_t byteToSend)
{
	vector<uint16_t> samples;

	auto readSize = _avrSpi.transceive(READ_SAMPLES);
	if (readSize > 0)
	{
		samples.reserve(readSize);

		for (int i = 0; i < readSize; ++i)
		{
			uint8_t data[2];
			data[0] = _avrSpi.transceive(byteToSend);
			data[1] = _avrSpi.transceive(byteToSend);
			uint16_t sample = (static_cast<uint16_t>(data[0]) << 8) + data[1];
			samples.push_back(sample);
		}
	}
	
	return samples;
}
