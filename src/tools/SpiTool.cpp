#include "avr_io/Spi.h"
#include "oscilloscope/SpiProfiler.h"
#include "oscilloscope/SampleReader.h"
#include "oscilloscope/SpiMessages.h"

#include <iostream>
#include <cstring>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono;

Spi spi;

void zeroBomb(unsigned count)
{
	cout << "Zero bomb" << endl;
	while (count-- > 0)
	{
		cout << int(spi.transceive(0)) << "; ";
	}
	cout << "\n----" << endl;
}

void printSamples(const vector<uint16_t> &samples)
{
	cout << "Read size: " << samples.size() << endl;
	cout << std::hex;
	for (const auto &sample : samples)
		cout << sample << "; ";
	cout << dec << endl;
}

void startSampling()
{
	cout << "Starting sampling.." << flush;
	spi.transceive(START_SAMPLING);
	cout << "Done" << endl;
}

void stopSampling()
{
	cout << "Stopping sampling.." << flush;
	spi.transceive(STOP_SAMPLING);
	cout << "Done" << endl;
}

void readSamplingState()
{
	spi.transceive(READ_STATUS);
	cout << "Sampling state: " << flush << int(spi.transceive(NO_CMD)) << endl;
}

void parseArg(const char *arg)
{
	if (strcmp(arg, "start") == 0)
	{
		startSampling();
	}
	else if (strcmp(arg, "stop") == 0)
	{
		stopSampling();
	}
	else if (strcmp(arg, "state") == 0)
	{
		readSamplingState();
	}
	else if (strcmp(arg, "zerobomb") == 0)
	{
		cout << "Zero bomb" << endl;
		zeroBomb(256);
		cout << "\n----" << endl;
	}
	else if (strcmp(arg, "read") == 0)
	{
		printSamples(SampleReader(spi).read());
	}
	else if (strcmp(arg, "readall") == 0)
	{
		SampleReader reader(spi);
		while(true)
			printSamples(reader.read());

	}
	else if (strcmp(arg, "profileread") == 0)
	{
		SampleReader reader(spi);
		unsigned byteCount = 0;
		unsigned zeroSizeReads = 0;
		unsigned badBufferSize = 0;
		const auto startTime = system_clock::now();
		unsigned printStep = 20'000;
		unsigned nextPrint = printStep;
		while(true)
		{
			this_thread::sleep_for(microseconds(1));
			const auto samples = reader.read();
			if (samples.size() == 0)
				zeroSizeReads += 1;
			else
			{
				if (samples.size() != 16) {
					cout << "Bad read: ";
					printSamples(samples);
					badBufferSize += 1;
				}
				byteCount += samples.size()*2;

				if (byteCount > nextPrint) {
					nextPrint += printStep;
					const auto elapsedTime = static_cast<double>(duration_cast<milliseconds>(system_clock::now() - startTime).count()) / 1000.;
					const double bitRate = static_cast<double>(byteCount) / static_cast<double>(elapsedTime);
					cout <<  byteCount << " bytes in " << elapsedTime << " seconds (" << bitRate << "Bps); " << zeroSizeReads << " empty reads; " << badBufferSize << " bad reads" << endl;
				}
			}
		}
	}
	else if (strcmp(arg, "incbyte") == 0)
	{
		cout << "Increment byte" << endl;
		spi.transceive(INC_BYTE);
	}
	else if (strcmp(arg, "readbyte") == 0)
	{
		spi.transceive(READ_BYTE);
		cout << "Byte value: " << flush << int(spi.transceive(NO_CMD)) << endl;
	}
	else if (strcmp(arg, "profilespi") == 0)
	{
		cout << "Profiling SPI.." << endl;
		TimedSpiProfiler(spi, std::chrono::milliseconds(10'000)).dump();
	}
	else
		cout << "Unknown argument \"" << arg << "\"" << endl;
}

int main(int argc, const char **argv)
{
	if (argc > 1)
		for (int i = 1; i < argc; ++i)
			parseArg(argv[i]);
	else
	{
		startSampling();

		readSamplingState();

		printSamples(SampleReader(spi).read());

		stopSampling();

		readSamplingState();
	}

	return 0;
}
