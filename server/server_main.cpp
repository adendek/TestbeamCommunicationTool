#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include "CommunicationMgr.h"

using namespace TestbeamCommunication;
using namespace std;
namespace asio = boost::asio;


int main(int argc, const char* argv[])
{
    unsigned port = 4050;
    if (argc > 1)
        port = atoi(argv[1]);
    cout << "Serving on port " << port << endl;

    try {
        asio::io_service io_service;
        CommunicationMgr server(io_service, 4050);
        io_service.run();
    }
    catch (std::exception& e) {
        cerr << e.what() << endl;
    }

    return 0;
}
