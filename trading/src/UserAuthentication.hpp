#pragma once

#include <Poco/Net/HTTPServerRequest.h>

namespace trading {

class UserAuthentication {
    public:
        static bool isAuthorizedUser(Poco::Net::HTTPServerRequest& request);
};

}
