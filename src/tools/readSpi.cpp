#include <iostream>
#include <cstring>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

using namespace std;

static const int CHANNEL = 0;
static int fd;

void setSpiMode(char mode)
{
	ioctl(fd, SPI_IOC_WR_MODE, &mode);
}

char getSpiMode()
{
	char mode = 0;
	ioctl(fd, SPI_IOC_RD_MODE, &mode);
	return mode;
}

uint8_t transceive(uint8_t writeData)
{
	delayMicroseconds(5);
	wiringPiSPIDataRW(CHANNEL, &writeData, 1);
	return writeData;
}

enum COMMAND : uint8_t
{
	NO_CMD = 0,
	START_SAMPLING = 1,
	STOP_SAMPLING = 2,
	READ_SAMPLES = 3,
	READ_STATUS = 4,
	COUNT = 10,
	INC_BYTE = 20,
	READ_BYTE = 21
};

void startSampling()
{
	cout << "Starting sampling.." << flush;
	transceive(START_SAMPLING);
	cout << "Done" << endl;
}

void stopSampling()
{
	cout << "Stopping sampling.." << flush;
	transceive(STOP_SAMPLING);
	cout << "Done" << endl;
}

void readSamplingState()
{
	transceive(READ_STATUS);
	cout << "Sampling state: " << int(transceive(NO_CMD)) << endl;
}

void readAllSamples()
{
	transceive(READ_SAMPLES);
	delay(10);

	cout << std::hex;
	while (true)
	{
		auto readSize = transceive(READ_SAMPLES);
		if (readSize == 0)
			cout << '.';
		for (int i = 0; i < readSize; ++i)
		{
			uint8_t data[2];
			data[0] = transceive(READ_SAMPLES);
			data[1] = transceive(READ_SAMPLES);
			uint16_t sample = (static_cast<uint16_t>(data[0]) << 8) + data[1];
			cout << sample << "; ";
		}
		cout << endl;
	}
	cout << std::dec;
}

void readSamples()
{
	transceive(READ_SAMPLES);
	delay(10);

	auto readSize = transceive(NO_CMD);
	cout << "Read size: " << int(readSize) << endl;
	cout << std::hex;
	if (readSize == 0)
		cout << '.';
	for (int i = 0; i < readSize; ++i)
	{
		uint8_t data[2];
		data[0] = transceive(NO_CMD);
		data[1] = transceive(NO_CMD);
		uint16_t sample = (static_cast<uint16_t>(data[0]) << 8) + data[1];
		cout << sample << "; ";
	}
	cout << std::dec;
	cout << endl;
}

void zeroBomb(unsigned count)
{
	cout << "Zero bomb" << endl;
	while (count-- > 0)
	{
		cout << int(transceive(0)) << "; ";
	}
	cout << "\n----" << endl;
}

void incByte()
{
	transceive(INC_BYTE);
}

void readByte()
{
	transceive(READ_BYTE);
	cout << "Byte value: " << int(transceive(NO_CMD)) << endl;

}

void profileSpi()
{
	cout << "Profiling SPI.." << endl;

	transceive(COUNT);
	const auto startTimestamp = micros();
	auto lastTimestamp = startTimestamp;
	unsigned errorCount = 0;
	unsigned byteCount = 0;
	unsigned i = 0;
	do {
		const uint8_t expected = i + 5;
		i = (i % 200) + 1;
		const auto received = transceive(i);
		if (received != expected)
		{
			errorCount += 1;
		}
		byteCount += 1;
		lastTimestamp = micros();
	} while (lastTimestamp - startTimestamp < 10'000'000);

	transceive(NO_CMD);


	double timeSecs = static_cast<double>(lastTimestamp - startTimestamp) / 1'000'000.;
	double errorRate = (static_cast<double>(errorCount) / static_cast<double>(byteCount)) * 100.;
	double byteRate = static_cast<double>(byteCount) / timeSecs;

	cout << "Transmitted " << byteCount << " bytes in " << timeSecs << " seconds" << endl;
	cout << "Throughtput: " << byteRate << " Bps = " << byteRate * 8 << " bps" << endl;
	cout << "Error count: " << errorCount << "/" << byteCount << " (" << errorRate << "% error rate)" << endl;
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
		zeroBomb(512);
	}
	else if (strcmp(arg, "read") == 0)
	{
		readSamples();
	}
	else if (strcmp(arg, "readall") == 0)
	{
		readAllSamples();
	}
	else if (strcmp(arg, "incbyte") == 0)
	{
		incByte();
	}
	else if (strcmp(arg, "readbyte") == 0)
	{
		readByte();
	}
	else if (strcmp(arg, "profilespi") == 0)
	{
		profileSpi();
	}
	else
		cout << "Unknown argument \"" << arg << "\"" << endl;
}

int main(int argc, const char **argv)
{
	cout << "Initializing" << endl ;
	fd = wiringPiSPISetup(CHANNEL, 1'500'000);
	cout << "File descriptor: " << fd << endl;

	setSpiMode(0);
	cout << "Spi mode: " << int(getSpiMode()) << endl;

	if (argc > 1) {
		for (int i = 1; i < argc; ++i)
		{
			parseArg(argv[i]);
			delay(10);
		}
	}
	else
	{
		startSampling();

		readSamplingState();

		readSamples();

		stopSampling();

		readSamplingState();
	}
	

	return 0;
}
