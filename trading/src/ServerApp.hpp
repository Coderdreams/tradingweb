#pragma once

#include <Poco/Util/OptionSet.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/Application.h>

namespace trading {

class ServerApp : public Poco::Util::ServerApplication
{
public:
    ServerApp();
    ~ServerApp();

protected:
    void initialize(Poco::Util::Application& self);
    void uninitialize();
    void defineOptions(Poco::Util::OptionSet& options);
    void handleHelp(const std::string& name, 
                    const std::string& value);
    int main(const std::vector<std::string>& args);

private:
    bool _helpRequested;
};

} // namespace trading