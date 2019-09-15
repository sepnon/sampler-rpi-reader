#include "SpiProfiler.h"
#include "SpiMessages.h"

#include <iostream>

using namespace std;
using namespace std::chrono;

SpiProfiler::SpiProfiler(Spi &spi)
	: _spi(spi)
{
	_spi.transceive(COUNT);
}

void SpiProfiler::sendByte(uint8_t byte)
{
	const auto expected = nextExpectedByte();
	const auto received = _spi.transceive(byte);

	_lastByteSent = byte;

	_byteCount += 1;
	if (received != expected)
		_errorCount += 1;
}

void SpiProfiler::sendByte()
{
	sendByte((_lastByteSent % 200) + 19);
}

SpiProfiler::~SpiProfiler()
{
	_spi.transceive(NO_CMD);
}

uint8_t SpiProfiler::nextExpectedByte() const
{
	return _lastByteSent + 5;
}


TimedSpiProfiler::TimedSpiProfiler(Spi &spi, milliseconds duration)
	: _profiler(spi)
{
	const auto startTime = system_clock::now();
	do {
		_profiler.sendByte();
		_duration = system_clock::now() - startTime;
	} while (_duration < duration);
}

void TimedSpiProfiler::dump()
{
	auto byteCount = _profiler.byteCount();
	auto errorCount = _profiler.errorCount();

	double timeSecs = duration_cast<seconds>(_duration).count();
	double errorRate = (static_cast<double>(errorCount) / static_cast<double>(byteCount)) * 100.;
	double byteRate = static_cast<double>(byteCount) / timeSecs;

	cout << "Transmitted " << byteCount << " bytes in " << timeSecs << " seconds" << endl;
	cout << "Throughtput: " << byteRate << " Bps = " << byteRate * 8 << " bps" << endl;
	cout << "Error count: " << errorCount << "/" << byteCount << " (" << errorRate << "% error rate)" << endl;
}
