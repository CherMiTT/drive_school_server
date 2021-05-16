#include "RequestHandler.h"
#include "server.h"
#include "Session.h"

void RequestHandler::handleRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    //TODO: check method post
    std::cout << "Request at " << req.getURI() << std::endl;

    if (req.getURI() == "/api/login") //if it's authorization request
    {
        handleAuthorizationRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/profile-info")
    {
        handleProfileInfoRequest(req, resp);
        return;
    }

    /*auto& stream = req.stream();
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
    out << "Hello, you logged in!";*/
}

void RequestHandler::handleAuthorizationRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    /*auto& stream = req.stream();
    const size_t len = req.getContentLength();
    char* buffer = new char[len + 1];
    memset(buffer, 0, len + 1);
    stream.read(buffer, len);

    std::string s(buffer);*/
    std::string s = getRequestString(req);
    Poco::JSON::Object::Ptr json = parseAuthorizationJson(s);

    std::string login = json->getValue<string>("login");
    std::string password = json->getValue<string>("password");

    resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    resp.setContentType("application/json");

    int id = MySQLHandler::getHandler()->getUserID(login, password);
    std::string jsonStr;
    if (id == -1) //no such user
    {
        cout << "Invalid request";
        jsonStr = "{ \"status\" : \"fail\" }";
    }
    else //such user found
    {
        //create and send token
        Session* s = new Session();
        s->id = id;
        cout << s->token << endl;
        Server::vSessions->push_back(s);
        jsonStr = "{ \"status\" : \"success\", \"token\" : \"" + s->token + "\" }";
    }

    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(jsonStr);
    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
    ostream& out = resp.send();
    out << result.toString();
}

void RequestHandler::handleProfileInfoRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    Poco::JSON::Parser parser;
    std::string s = getRequestString(req);
    Poco::Dynamic::Var json = parser.parse(s);
    Poco::JSON::Object::Ptr object = json.extract<Poco::JSON::Object::Ptr>();
    
    std::string token = object->getValue<string>("token");
    int id = -1;
    for (auto o : *(Server::vSessions))
    {
        if (o->token.compare(token) == 0)
        {
            id = o->id;
            break;
        }
    }

    std::string jsonStr;
    if (id == -1)
    {
        cout << "Invalid token";
        jsonStr = "{ \"status\" : \"fail\" }";
    }
    else
    {
        jsonStr = "{ \"status\" : \"success\" }";

    }
}

Poco::JSON::Object::Ptr RequestHandler::parseAuthorizationJson(string& json)
{
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(json);
    //TODO: check object or an array
    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
    return object;
}

std::string RequestHandler::getRequestString(Poco::Net::HTTPServerRequest& req)
{
    auto& stream = req.stream();
    const size_t len = req.getContentLength();
    char* buffer = new char[len + 1];
    memset(buffer, 0, len + 1);
    stream.read(buffer, len);
    std::string s(buffer);
    return s;
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

    return new RequestHandler;
}