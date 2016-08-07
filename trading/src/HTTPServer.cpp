#include "HTTPServer.hpp"
#include "HandlerFactory.hpp"

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/ServerSocket.h>

namespace trading {

void HTTPServer::start(unsigned short port) {
    start(port, true);
}

void HTTPServer::start(unsigned short port, bool keepAlive) {
    Poco::Net::ServerSocket svs(port);
    Poco::Net::HTTPServerParams* pParams = new Poco::Net::HTTPServerParams;
	pParams->setKeepAlive(keepAlive);
    _srv = std::make_unique<Poco::Net::HTTPServer>(new trading::HandlerFactory(), 
        svs, pParams);
    _srv->start();
}

void HTTPServer::stop() {
    _srv->stop();
}

} // namespace trading