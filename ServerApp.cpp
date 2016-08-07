#include "ServerApp.hpp"
#include "HTTPServer.hpp"

#include <Poco/Util/Option.h>
#include <Poco/Util/HelpFormatter.h>
#include <iostream>

namespace trading {

using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;
using Poco::Util::ServerApplication;
using Poco::Util::Application;

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
        "A trading app");
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
        trading::HTTPServer srv;
        srv.start(port);
        waitForTerminationRequest();
        srv.stop();
    }
    return Application::EXIT_OK;
}

} // namespace trading
