#include "datareader.h"

#include <iostream>
#include <chrono>
#include <math.h>

#include "SampleReader.h"
#include "driver.h"

using namespace std;
using namespace std::chrono;

DataReader::DataReader()
{
	init();
	zeroBomb(512);
}

DataReader::~DataReader()
{
    stop();
}

void DataReader::start()
{
    _thread = std::thread([this]{ run(); });
}

void DataReader::stop()
{
    _running = false;
    if (_thread.joinable())
	    _thread.join();
}

std::vector<Sample> DataReader::samples()
{
	std::vector<Sample> takenSamples;
	if (nextReadBlock != nextWriteBlock) {
		std::swap(_data[nextReadBlock], takenSamples);
		nextReadBlock += 1;
		nextReadBlock %= BlocksCount;
	}
	return takenSamples;
}

void DataReader::run()
{
    _running = true;
    SampleReader reader;
    this_thread::sleep_for(milliseconds(100));
    const auto startTime = system_clock::now();
    unsigned byteCount = 0;
    unsigned x = 0;
    while (_running) {
	    if ((nextWriteBlock + 1) % BlocksCount == nextReadBlock) {
		    this_thread::sleep_for(milliseconds(1));
		    continue;
	    }

	    const auto samples = reader.read();
	    if (samples.empty())
		    continue;

	    std::vector<Sample> stampedSamples;
	    for (const auto &sample : samples)
	    {
		    // const auto x = duration_cast<microseconds>(system_clock::now() - startTime).count();
		    stampedSamples.push_back({x, sample});
		    x += 1;
		    byteCount += 2;
		    if (byteCount % 100'000 == 0) {
			    const auto elapsedTime = static_cast<double>(duration_cast<milliseconds>(system_clock::now() - startTime).count()) / 1000.;
			    const double bitRate = static_cast<double>(byteCount) / static_cast<double>(elapsedTime);
			    cout <<  byteCount << " bytes in " << elapsedTime << " seconds (" << bitRate << "Bps)" << endl;
		    }
	    }
	    std::swap(_data[nextWriteBlock], stampedSamples);
	    nextWriteBlock += 1;
	    nextWriteBlock %= BlocksCount;
    }
}
