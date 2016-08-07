#include "ServerApp.hpp"
#include "HandlerFactory.hpp"

#include <Poco/Util/Option.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/DateTimeFormat.h>
#include <iostream>

namespace trading {

using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;
using Poco::Util::ServerApplication;
using Poco::Util::Application;
using Poco::Net::ServerSocket;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::DateTimeFormat;

ServerApp::ServerApp(): _helpRequested(false) {}

ServerApp::~ServerApp() {}

void ServerApp::initialize(Application& self)
{
    loadConfiguration();
    ServerApplication::initialize(self);
}

void ServerApp::uninitialize()
{
    ServerApplication::uninitialize();
}

void ServerApp::defineOptions(OptionSet& options)
{
    ServerApplication::defineOptions(options);

    options.addOption(
    Option("help", "h", "display argument help information")
        .required(false)
        .repeatable(false)
        .callback(OptionCallback<ServerApp>(
            this, &ServerApp::handleHelp)));
}

void ServerApp::handleHelp(const std::string& name, 
                const std::string& value)
{
    HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("OPTIONS");
    helpFormatter.setHeader(
        "A web server that serves the current date and time.");
    helpFormatter.format(std::cout);
    stopOptionsProcessing();
    _helpRequested = true;
}

int ServerApp::main(const std::vector<std::string>& args)
{
    if (!_helpRequested)
    {
        unsigned short port = (unsigned short)
            config().getInt("HTTPTimeServer.port", 9980);
        std::string format(
            config().getString("HTTPTimeServer.format", 
                                DateTimeFormat::SORTABLE_FORMAT));

        ServerSocket svs(port);
        HTTPServer srv(new trading::HandlerFactory(format), 
            svs, new HTTPServerParams);
        srv.start();
        waitForTerminationRequest();
        srv.stop();
    }
    return Application::EXIT_OK;
}

} // namespace trading
