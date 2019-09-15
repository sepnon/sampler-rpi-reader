#ifndef DATAREADER_H
#define DATAREADER_H

#include <thread>
#include <vector>
#include <avr_io/Spi.h>

struct Sample
{
	unsigned timestamp = 0;
	unsigned value = 0;
};

class DataReader
{
public:
    ~DataReader();

    void start();
    void stop();

    std::vector<Sample> samples();

private:
    void run();
    volatile bool _running = false;
    std::thread _thread;

    Spi _spi;

    static constexpr auto BlocksCount = 32;
    std::array<std::vector<Sample>, BlocksCount> _data;
    unsigned nextWriteBlock = 0;
    unsigned nextReadBlock = 0;
};

#endif // DATAREADER_H
