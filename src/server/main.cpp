#include <iostream>
#include <thread>
#include <chrono>

#include <boost/asio.hpp>

#include "datareader.h"

using namespace std;
using namespace boost::asio;

int main()
{
    DataReader reader;

    try {
	io_service io;

        ip::tcp::acceptor acceptor(io, ip::tcp::endpoint(ip::tcp::v4(), 9090));

        while(true) {
            cout << "Accepting connections.." << endl;

            ip::tcp::socket socket(io);
            acceptor.accept(socket);

            cout << "Connection accepted" << endl;

	    reader.start();

	    try {
		    while (true) {
			    const auto samples = reader.samples();
			    for (const auto &sample : samples) {
				    boost::asio::write(socket, boost::asio::buffer(&sample.value, sizeof(sample.value)));
			    }
		    }
	    }
            catch (exception &e) {
                cout << "Exception on write: " << e.what() << endl;
            }

	    reader.stop();
        }
    }
    catch (const exception &e) {
        cout << "Exception: " << e.what() << endl;
    }

    system("PAUSE");

    return 0;
}
