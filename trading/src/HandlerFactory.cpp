#include "HandlerFactory.hpp"
#include "handler/Home.hpp"
#include "handler/SignUp.hpp"
#include "handler/Login.hpp"
#include "handler/RegisterTrader.hpp"
#include "handler/StaticContent.hpp"
#include "handler/Dashboard.hpp"
#include <boost/algorithm/string/predicate.hpp>

namespace trading {

Poco::Net::HTTPRequestHandler* HandlerFactory::createRequestHandler(
    const Poco::Net::HTTPServerRequest& request)
{
    if (request.getURI() == "/")
        return new trading::handler::Home();
    else if (request.getURI() == "/login")
        return new trading::handler::Login();
    else if (request.getURI() == "/signup")
        return new trading::handler::SignUp();
    else if (request.getURI() == "/dashboard")
        return new trading::handler::Dashboard();
    else if (request.getURI() == "/registerTrader")
        return new trading::handler::RegisterTrader();
    else if (boost::starts_with(request.getURI(), "/static"))
        return new trading::handler::StaticContent();
    else
        return 0;
}

} // namespace trading