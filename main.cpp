#include <iostream>
#include <cmath>
#include "Logging.h"

#include <boost/asio.hpp>


#define DBG		    Logger::D
#define INFO		Logger::I
#define WARN		Logger::W
#define CRITICAL	Logger::E
#define PY			Logger::PY
#define CDBG        Logger::C


int main()
{
    DBG("ffffffffff %d", sizeof(boost::asio::ip::address_v4));

    std::cout << "Hello World!" << std::endl;
    DBG("hi blablabla");
    Logger::ResetInstance("test.log", LOGLEVEL_DEBUG);
    DBG("fffffffff %s", M_PI);
    DBG("test log after reset log to ofstream file");
    return 0;
}

