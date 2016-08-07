#include "HTTPServer.hpp"
#include "HandlerFactory.hpp"

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/ServerSocket.h>

namespace trading {

void HTTPServer::start(unsigned short port) {
    Poco::Net::ServerSocket svs(port);
    _srv = new Poco::Net::HTTPServer(new trading::HandlerFactory(), 
        svs, new Poco::Net::HTTPServerParams);
    _srv->start();
}

void HTTPServer::stop() {
    _srv->stop();
    delete _srv;
}

} // namespace trading