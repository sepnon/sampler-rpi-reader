#ifndef DATAREADER_H
#define DATAREADER_H

#include <thread>
#include <vector>

struct Sample
{
	unsigned timestamp = 0;
	unsigned value = 0;
};

class DataReader
{
public:
    DataReader();
    ~DataReader();

    void start();
    void stop();

    std::vector<Sample> samples();

private:
    void run();
    volatile bool _running = false;
    std::thread _thread;

    static constexpr auto BlocksCount = 32;
    std::array<std::vector<Sample>, BlocksCount> _data;
    unsigned nextWriteBlock = 0;
    unsigned nextReadBlock = 0;
};

#endif // DATAREADER_H
