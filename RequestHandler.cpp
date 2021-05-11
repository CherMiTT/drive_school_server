#include "RequestHandler.h"
#include "Server.h"

void AuthorizationRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    //TODO: check method post
    //std::cout << "Request at " << req.getURI() << std::endl;

    auto& stream = req.stream();
    const size_t len = req.getContentLength();
    char* buffer = new char[len + 1];
    memset(buffer, 0, len + 1);
    stream.read(buffer, len);

    std::string s(buffer);
    Poco::JSON::Object::Ptr json = parseAuthorizationJson(s);
    
    std::string login = json->getValue<string>("login");
    std::string password = json->getValue<string>("password");
    std::cout << "login = " << login << std::endl << "password = " << password << std::endl;

    //TODO: Check login and password
    resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK); //HTTP_OK
    resp.setContentType("application/json");

    ostream& out = resp.send();
    out << "Hello, you logged in!";
}

Poco::JSON::Object::Ptr AuthorizationRequestHandler::parseAuthorizationJson(string& json)
{
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(json);
    //TODO: check object or an array
    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
    return object;
}

Poco::Net::HTTPRequestHandler* RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest& req)
{
    std::cout << "Request received" << std::endl;
    std::string uri = req.getURI();
    if (uri.substr(0, 5).compare("/api/") != 0)
    {
        std::cout << "Request not at api! Not supported yet!" << std::endl;
        return nullptr;
    }

    if (uri.compare("/api/login") == 0)
    {
        std::cout << "Authorization request to api! Handling!" << std::endl;
        return new AuthorizationRequestHandler;
    }

    return nullptr;
}

