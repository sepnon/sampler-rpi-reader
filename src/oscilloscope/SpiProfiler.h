#ifndef SPIPROFILER_H
#define SPIPROFILER_H

#include <avr_io/Spi.h>
#include <chrono>

class SpiProfiler
{
	public:
		SpiProfiler(Spi &spi);
		~SpiProfiler();

		void sendByte(uint8_t byte);
		void sendByte();

		unsigned byteCount() const { return _byteCount; }
		unsigned errorCount() const { return _errorCount; }

	private:
		uint8_t nextExpectedByte() const;

		Spi &_spi;

		unsigned _errorCount = 0;
		unsigned _byteCount = 0;
		unsigned _lastByteSent = 0;
};

class TimedSpiProfiler
{
	public:
		TimedSpiProfiler(Spi &spi, std::chrono::milliseconds duration);

		void dump();

	private:
		SpiProfiler _profiler;

		std::chrono::duration<double> _duration;
};

#endif // SPIPROFILER_H
