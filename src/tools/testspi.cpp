#include <iostream>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

using namespace std;

static const int CHANNEL = 0;
static int fd;

static const int sleep_time = 1;
static const int tests_count = 1000;

void transceive(uint8_t *byte)
{
	// cout << "[" << int(*byte) << ">; ";
	delay(sleep_time);
	wiringPiSPIDataRW(CHANNEL, byte, 1);
	// cout << int(*byte) << "<] ";
}

class Test
{
	public:
		Test(int testsCount)
		{
			for(int i = 0; i < testsCount; ++i)
			{
				testSum(i*2, i);
			}
		}
		
		void testSum(uint8_t v1, uint8_t v2)
		{
			_testsCount += 1;

			const uint8_t expectedResult = v1 + v2;

			// cout << "Testing " << int(v1) << " + " << int(v2) << ": ";

			transceive(&v1);
			transceive(&v2);

			v1 = 0;
			transceive(&v1);
			v2 = 0;
			transceive(&v2);

			uint8_t result = 0;
			transceive(&result);

			checkResult(result, expectedResult);
			// cout << endl;
		}

		void print() const
		{
			cout << "Error count: " << _errorCount << "/" << _testsCount << endl << endl;
		}

	protected:
		void checkResult(uint8_t received, uint8_t expected)
		{
			cout << std::hex;
			if (received != expected) {
				if (_errorCount < 10)
					cout << "Received " << (int)received << ", expected " << (int)expected << endl;
				else if (_errorCount == 10)
					cout << "..." << endl;
				_errorCount += 1;
			}
			cout << std::dec;

		}

	private:
		int _errorCount = 0;
		int _testsCount = 0;
};

void testSpiMode(char mode, int testsCount)
{
	ioctl(fd, SPI_IOC_WR_MODE, &mode);
	ioctl(fd, SPI_IOC_RD_MODE, &mode);
	cout << "SPI mode: " << static_cast<unsigned>(mode) << endl;

	Test test(testsCount);
	test.print();
}

int main(int argc, char **argv)
{
	cout << "Initializing" << endl;
	fd = wiringPiSPISetup(CHANNEL, 100'000);
	cout << "File descriptor: " << fd << endl;

	if (argc <= 1) {
		for(int i = 0; i < 4; ++i)
			testSpiMode(i, tests_count);
	}
	else {
		for (int i = 1; i < argc; ++i) {
			switch (argv[i][0]) {
				case '0':
				case '1':
				case '2':
				case '3':
					testSpiMode(argv[i][0] - '0', tests_count);
					break;
				default:
					cout << "Unknown argument \"" << argv[i] << "\"" << endl;
			}
		}
	}

	return 0;
}
