#include "RequestHandler.h"
#include "server.h"
#include "Session.h"
#include "dataBaseHandler.h"

void RequestHandler::handleRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    //TODO: check method post
    std::cout << "Request at " << req.getURI() << std::endl;

    if (req.getURI() == "/api/login") //if it's authorization request
    {
        handleAuthorizationRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/user-info")
    {
        handleProfileInfoRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/add/user")
    {
        handleAddUserRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/all-users")
    {
        handleAllUsersRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/all-students")
    {
        handleStudentsRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/all-instructors")
    {
        handleInstructorsRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/all-admins")
    {
        handleAdminsRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/all-groups")
    {
        handleGroupsRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/group-list")
    {
        handleGroupsRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/all-cars")
    {
        handleCarsRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/all-rooms")
    {
        handleRoomsRequest(req, resp);
        return;
    }

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
    Poco::JSON::Object::Ptr json = parseObjectJson(s);

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

        struct User user = MySQLHandler::getHandler()->getUserInfo(s);
        s->userInfoJsonString = "\"first_name\" : \"" + user.first_name + "\", \
\"middle_name\" : \"" + user.middle_name + "\", \
\"last_name\" : \"" + user.last_name +"\", \
\"role\" : \"" + user.role + "\" }";
    }

    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(jsonStr);
    ostream& out = resp.send();
    out << result.toString();
}

void RequestHandler::handleProfileInfoRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    Poco::JSON::Object::Ptr object = parseObjectJson(getRequestString(req));

    std::string token = object->getValue<string>("token");
    int id = -1;
    std::string userInfo;
    for (auto o : *(Server::vSessions))
    {
        if (o->token.compare(token) == 0)
        {
            id = o->id;
            userInfo = o->userInfoJsonString;
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
        jsonStr = "{ \"status\" : \"success\", ";
        jsonStr += userInfo;
    }

    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(jsonStr);
    ostream& out = resp.send();
    out << result.toString();
}

void RequestHandler::handleAddUserRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    //TODO parsing into function
    std::string jsonStr = "";
    std::string role = "";
    if (CheckTokenAndRole(req, jsonStr, role))
    {
        Poco::JSON::Object::Ptr object = parseObjectJson(getRequestString(req));

        struct User newUser;
        newUser.first_name = object->getValue<string>("name");
        newUser.middle_name = object->getValue<string>("middle_name");
        newUser.last_name = object->getValue<string>("last_name");
        std::string login = object->getValue<string>("login");
        std::string password = object->getValue<string>("password");
        std::string phone = object->getValue<string>("phone_number");
        std::string email = object->getValue<string>("email");
        std::string pass = object->getValue<string>("pass");
        newUser.role = object->getValue<string>("role");
        jsonStr = "{ \"status\" : \"success\" }";
        MySQLHandler::getHandler()->addUser(newUser, login, password, phone, email, pass);
    }

    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(jsonStr);
    ostream& out = resp.send();
    out << result.toString();
}

void RequestHandler::handleAllUsersRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    std::string jsonStr = "";
    std::string role = "";
    if (CheckTokenAndRole(req, jsonStr, role))
    {
        int count;
        jsonStr = MySQLHandler::getHandler()->getAllUsers(count);
        if (count == 0) jsonStr = "{ \"status\" : \"success\" }";
    }
    std::cout << jsonStr << std::endl;

    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(jsonStr);
    ostream& out = resp.send();
    out << result.toString();

}

void RequestHandler::handleAdminsRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    std::string jsonStr = "";
    std::string role = "";
    if (CheckTokenAndRole(req, jsonStr, role))
    {
        int count = 0;
        jsonStr = MySQLHandler::getHandler()->getAdmins(count);
        if (count == 0) jsonStr = "{ \"status\" : \"success\" }";
    }
    std::cout << jsonStr << std::endl;

    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(jsonStr);
    ostream& out = resp.send();
    out << result.toString();
}

void RequestHandler::handleInstructorsRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    std::string jsonStr = "";
    std::string role = "";
    if (CheckTokenAndRole(req, jsonStr, role))
    {
        int count = 0;
        jsonStr = MySQLHandler::getHandler()->getInstructors(count);
        if (count == 0) jsonStr = "{ \"status\" : \"success\" }";
    }
    std::cout << jsonStr << std::endl;

    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(jsonStr);
    ostream& out = resp.send();
    out << result.toString();
}

void RequestHandler::handleStudentsRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    std::string jsonStr = "";
    std::string role = "";
    if (CheckTokenAndRole(req, jsonStr, role))
    {
        int count = 0;
        jsonStr = MySQLHandler::getHandler()->getStudents(count);
        if (count == 0) jsonStr = "{ \"status\" : \"success\" }";
    }
    std::cout << jsonStr << std::endl;

    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(jsonStr);
    ostream& out = resp.send();
    out << result.toString();
}

void RequestHandler::handleGroupsRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    std::string jsonStr = "";
    std::string role = "";
    if (CheckTokenAndRole(req, jsonStr, role))
    {
        int count = 0;
        jsonStr = MySQLHandler::getHandler()->getGroups(count);
        if (count == 0) jsonStr = "{ \"status\" : \"success\" }";
    }
    std::cout << jsonStr << std::endl;

    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(jsonStr);
    ostream& out = resp.send();
    out << result.toString();
}

void RequestHandler::handleCarsRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    std::string jsonStr = "";
    std::string role = "";
    if (CheckTokenAndRole(req, jsonStr, role))
    {
        int count = 0;
        jsonStr = MySQLHandler::getHandler()->getCars(count);
        if (count == 0) jsonStr = "{ \"status\" : \"success\" }";
    }
    std::cout << jsonStr << std::endl;

    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(jsonStr);
    ostream& out = resp.send();
    out << result.toString();
}

void RequestHandler::handleRoomsRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    std::string jsonStr = "";
    std::string role = "";
    if (CheckTokenAndRole(req, jsonStr, role))
    {
        int count = 0;
        jsonStr = MySQLHandler::getHandler()->getRooms(count);
        if (count == 0) jsonStr = "{ \"status\" : \"success\" }";
    }
    std::cout << jsonStr << std::endl;
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(jsonStr);
    ostream& out = resp.send();
    out << result.toString();
}

bool RequestHandler::CheckTokenAndRole(Poco::Net::HTTPServerRequest& req, std::string& jsonStr, std::string& role)
{
    /*Poco::JSON::Parser parser;
    std::string s = getRequestString(req);
    Poco::Dynamic::Var json = parser.parse(s);
    Poco::JSON::Object::Ptr object = json.extract<Poco::JSON::Object::Ptr>();
    */
    Poco::JSON::Object::Ptr object = parseObjectJson(getRequestString(req));

    std::string token = object->getValue<string>("token");
    int id = -1;
    std::string userInfo = "";
    for (auto o : *(Server::vSessions))
    {
        if (o->token.compare(token) == 0)
        {
            id = o->id;
            userInfo = o->userInfoJsonString;
            break;
        }
    }
    if (userInfo != "")
    {
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var info = parser.parse("{" + userInfo);
        Poco::JSON::Object::Ptr infoJson = info.extract<Poco::JSON::Object::Ptr>();
        role = infoJson->getValue<string>("role");
    }

    if (id == -1)
    {
        cout << "Invalid token";
        jsonStr = "{ \"status\" : \"fail\" }";
        return false;
    }
    else if (role != "a")
    {
        cout << "Not admin";
        jsonStr = "{ \"status\" : \"fail\" }";
        return false;
    }
    return true;
}

Poco::JSON::Object::Ptr RequestHandler::parseObjectJson(string& json)
{
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(json);
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