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
    if (req.getURI() == "/api/add/car")
    {
        handleAddCarRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/add/room")
    {
        handleAddRoomRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/add/group")
    {
        handleAddGroupRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/all-lessons")
    {
        handleAllLessonsRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/lesson-group-list")
    {
        handleGroupsRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/lesson-instr-list")
    {
        handleInstructorsRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/lesson-rooms-list")
    {
        handleRoomsRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/add/lesson")
    {
        handleAddLessonRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/all-tests")
    {
        handleAllTestsRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/add/test")
    {
        handleAddTestRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/delete/user")
    {
        handleDeleteUserRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/delete/group")
    {
        handleDeleteGroupRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/delete/room")
    {
        handleDeleteRoomRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/delete/car")
    {
        handleDeleteCarRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/delete/lesson")
    {
        handleDeleteLessonRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/delete/test")
    {
        handleDeleteTestRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/student/schedule")
    {
        handleStudentScheduleRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/student/group")
    {
        handleStudentGroupRequest(req, resp);
        return;
    }  
    if (req.getURI() == "/api/student/test-ids")
    {
        handleAllTestsRequest(req, resp);
        return;
    }
    if (req.getURI() == "/api/student/test")
    {
        handleStudentTestRequest(req, resp);
        return;
    }
}

void RequestHandler::handleAuthorizationRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
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
    Poco::JSON::Object::Ptr object;
    if (CheckTokenAndRole(req, jsonStr, role, object))
    {

        struct User newUser;
        newUser.first_name = object->getValue<string>("name");
        newUser.middle_name = object->getValue<string>("middle_name");
        newUser.last_name = object->getValue<string>("last_name");
        newUser.login = object->getValue<string>("login");
        newUser.password = object->getValue<string>("password");
        newUser.phone = object->getValue<string>("phone_number");
        newUser.email = object->getValue<string>("email");
        newUser.pass_number = object->getValue<string>("pass");
        newUser.role = object->getValue<string>("role");
        jsonStr = "{ \"status\" : \"success\" }";
        MySQLHandler::getHandler()->addUser(newUser);
        if (role == "s") MySQLHandler::getHandler()->addStudent(newUser, object->getValue<string>("group"));
        if (role == "i") MySQLHandler::getHandler()->addInstructor(newUser, object->getValue<int>("salary"));
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
    Poco::JSON::Object::Ptr ptr;
    if (CheckTokenAndRole(req, jsonStr, role, ptr))
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
    Poco::JSON::Object::Ptr ptr;
    if (CheckTokenAndRole(req, jsonStr, role, ptr))
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
    Poco::JSON::Object::Ptr ptr;
    if (CheckTokenAndRole(req, jsonStr, role, ptr))
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
    Poco::JSON::Object::Ptr ptr;
    if (CheckTokenAndRole(req, jsonStr, role, ptr))
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
    Poco::JSON::Object::Ptr ptr;
    if (CheckTokenAndRole(req, jsonStr, role, ptr))
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
    Poco::JSON::Object::Ptr ptr;
    if (CheckTokenAndRole(req, jsonStr, role, ptr))
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
    Poco::JSON::Object::Ptr ptr;
    if (CheckTokenAndRole(req, jsonStr, role, ptr))
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

void RequestHandler::handleAddCarRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    std::string jsonStr = "";
    std::string role = "";
    Poco::JSON::Object::Ptr ptr;
    if (CheckTokenAndRole(req, jsonStr, role, ptr))
    {
        MySQLHandler::getHandler()->addCar(ptr->getValue<string>("model"), ptr->getValue<string>("plate"), ptr->getValue<int>("status"));
        jsonStr = "{ \"status\" : \"success\" }";
    }
    std::cout << jsonStr << std::endl;
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(jsonStr);
    ostream& out = resp.send();
    out << result.toString();
}

void RequestHandler::handleAddRoomRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    std::string jsonStr = "";
    std::string role = "";
    Poco::JSON::Object::Ptr ptr;
    if (CheckTokenAndRole(req, jsonStr, role, ptr))
    {
        MySQLHandler::getHandler()->addRoom(ptr->getValue<int>("room"));
        jsonStr = "{ \"status\" : \"success\" }";
    }
    std::cout << jsonStr << std::endl;
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(jsonStr);
    ostream& out = resp.send();
    out << result.toString();
}

void RequestHandler::handleAddGroupRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    std::string jsonStr = "";
    std::string role = "";
    Poco::JSON::Object::Ptr ptr;
    if (CheckTokenAndRole(req, jsonStr, role, ptr))
    {
        MySQLHandler::getHandler()->addGroup(ptr->getValue<string>("name"));
        jsonStr = "{ \"status\" : \"success\" }";
    }
    std::cout << jsonStr << std::endl;
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(jsonStr);
    ostream& out = resp.send();
    out << result.toString();
}

void RequestHandler::handleAllLessonsRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    std::string jsonStr = "";
    std::string role = "";
    Poco::JSON::Object::Ptr ptr;
    if (CheckTokenAndRole(req, jsonStr, role, ptr))
    {
        int count;
        jsonStr = MySQLHandler::getHandler()->getAllLessons(count);
        if (count == 0) jsonStr = "{ \"status\" : \"success\" }";
    }
    std::cout << jsonStr << std::endl;
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(jsonStr);
    ostream& out = resp.send();
    out << result.toString();

}

void RequestHandler::handleAddLessonRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    std::string jsonStr = "";
    std::string role = "";
    Poco::JSON::Object::Ptr ptr;
    if (CheckTokenAndRole(req, jsonStr, role, ptr))
    {
        MySQLHandler::getHandler()->addLesson(ptr);
        jsonStr = "{ \"status\" : \"success\" }";
    }
    std::cout << jsonStr << std::endl;
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(jsonStr);
    ostream& out = resp.send();
    out << result.toString();
}

void RequestHandler::handleAllTestsRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    std::string jsonStr = "";
    std::string role = "";
    Poco::JSON::Object::Ptr ptr;
    if (CheckTokenAndRole(req, jsonStr, role, ptr))
    {
        int count;
        jsonStr = MySQLHandler::getHandler()->getAllTests(count);
        if(count == 0) jsonStr = "{ \"status\" : \"success\" }";
    }
    if (role == "s" || role == "i")
    {
        int count;
        jsonStr = MySQLHandler::getHandler()->getAllTests(count);
        if (count == 0) jsonStr = "{ \"status\" : \"success\" }";
    }
    std::cout << jsonStr << std::endl;
    Poco::Dynamic::Var result;
    try
    {
        Poco::JSON::Parser parser;
        result = parser.parse(jsonStr);
    }
    catch (const Poco::Exception& exc)
    {
        std::cout << "Exception while parsing json:" << exc.name() << " : " << exc.displayText() << std::endl;
    }
    ostream& out = resp.send();
    out << result.toString();
}

void RequestHandler::handleAddTestRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    std::string jsonStr = "";
    std::string role = "";
    Poco::JSON::Object::Ptr ptr;
    if (CheckTokenAndRole(req, jsonStr, role, ptr))
    {
        int id;
        std::string token = ptr->getValue<string>("token");
        for (auto o : *(Server::vSessions))
        {
            if (o->token.compare(token) == 0)
            {
                id = o->id;
                break;
            }
        }
        MySQLHandler::getHandler()->addTest(ptr, id);
        jsonStr = "{ \"status\" : \"success\" }";
    }
    std::cout << jsonStr << std::endl;
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(jsonStr);
    ostream& out = resp.send();
    out << result.toString();
}

void RequestHandler::handleDeleteUserRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    std::string jsonStr = "";
    std::string role = "";
    Poco::JSON::Object::Ptr ptr;
    if (CheckTokenAndRole(req, jsonStr, role, ptr))
    {
        std::string login = ptr->getValue<std::string>("login");
        MySQLHandler::getHandler()->deleteUser(login);
        jsonStr = "{ \"status\" : \"success\" }";
    }
    std::cout << jsonStr << std::endl;
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(jsonStr);
    ostream& out = resp.send();
    out << result.toString();
}

void RequestHandler::handleDeleteGroupRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    std::string jsonStr = "";
    std::string role = "";
    Poco::JSON::Object::Ptr ptr;
    if (CheckTokenAndRole(req, jsonStr, role, ptr))
    {
        int group = ptr->getValue<int>("group");
        MySQLHandler::getHandler()->deleteGroup(group);
        jsonStr = "{ \"status\" : \"success\" }";
    }
    std::cout << jsonStr << std::endl;
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(jsonStr);
    ostream& out = resp.send();
    out << result.toString();
}

void RequestHandler::handleDeleteRoomRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    std::string jsonStr = "";
    std::string role = "";
    Poco::JSON::Object::Ptr ptr;
    if (CheckTokenAndRole(req, jsonStr, role, ptr))
    {
        int room = ptr->getValue<int>("room");
        MySQLHandler::getHandler()->deleteRoom(room);
        jsonStr = "{ \"status\" : \"success\" }";
    }
    std::cout << jsonStr << std::endl;
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(jsonStr);
    ostream& out = resp.send();
    out << result.toString();
}

void RequestHandler::handleDeleteCarRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    std::string jsonStr = "";
    std::string role = "";
    Poco::JSON::Object::Ptr ptr;
    if (CheckTokenAndRole(req, jsonStr, role, ptr))
    {
        std::string plate = ptr->getValue<std::string>("plate");
        MySQLHandler::getHandler()->deleteCar(plate);
        jsonStr = "{ \"status\" : \"success\" }";
    }
    std::cout << jsonStr << std::endl;
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(jsonStr);
    ostream& out = resp.send();
    out << result.toString();
}

void RequestHandler::handleDeleteLessonRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    std::string jsonStr = "";
    std::string role = "";
    Poco::JSON::Object::Ptr ptr;
    if (CheckTokenAndRole(req, jsonStr, role, ptr))
    {
        int id = ptr->getValue<int>("id");
        MySQLHandler::getHandler()->deleteLesson(id);
        jsonStr = "{ \"status\" : \"success\" }";
    }
    std::cout << jsonStr << std::endl;
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(jsonStr);
    ostream& out = resp.send();
    out << result.toString();
}
void RequestHandler::handleDeleteTestRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    std::string jsonStr = "";
    std::string role = "";
    Poco::JSON::Object::Ptr ptr;
    if (CheckTokenAndRole(req, jsonStr, role, ptr))
    {
        int id = ptr->getValue<int>("id");
        MySQLHandler::getHandler()->deleteLesson(id);
        jsonStr = "{ \"status\" : \"success\" }";
    }
    std::cout << jsonStr << std::endl;
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(jsonStr);
    ostream& out = resp.send();
    out << result.toString();
}

void RequestHandler::handleStudentGroupRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    Poco::JSON::Object::Ptr object = parseObjectJson(getRequestString(req));
    std::string token = object->getValue<string>("token");
    int id = -1;
    std::string userInfo = "";
    std::string jsonStr = "";
    for (auto o : *(Server::vSessions))
    {
        if (o->token.compare(token) == 0)
        {
            id = o->id;
            userInfo = o->userInfoJsonString;
            break;
        }
    }
    std::string role;
    if (userInfo != "")
    {
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var info = parser.parse("{" + userInfo);
        Poco::JSON::Object::Ptr infoJson = info.extract<Poco::JSON::Object::Ptr>();
        role = infoJson->getValue<string>("role");
    }
    if (role == "s")
    {
        jsonStr = MySQLHandler::getHandler()->getStudentGroup(id);
    }
    std::cout << jsonStr << std::endl;
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(jsonStr);
    ostream& out = resp.send();
    out << result.toString();
}

void RequestHandler::handleStudentTestRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    std::string jsonStr = "";
    std::string role = "";
    Poco::JSON::Object::Ptr ptr;
    if (CheckTokenAndRole(req, jsonStr, role, ptr))
    {
        int count;
        int id = ptr->getValue<int>("id");
        jsonStr = MySQLHandler::getHandler()->getTest(id);
    }
    if (role == "s" || role == "i")
    {
        int id = ptr->getValue<int>("id");
        jsonStr = MySQLHandler::getHandler()->getTest(id);
    }
    std::cout << jsonStr << std::endl;
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(jsonStr);
    ostream& out = resp.send();
    out << result.toString();
}

void RequestHandler::handleStudentScheduleRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    Poco::JSON::Object::Ptr object = parseObjectJson(getRequestString(req));
    std::string token = object->getValue<string>("token");
    int id = -1;
    std::string userInfo = "";
    std::string jsonStr = "";
    for (auto o : *(Server::vSessions))
    {
        if (o->token.compare(token) == 0)
        {
            id = o->id;
            userInfo = o->userInfoJsonString;
            break;
        }
    }
    std::string role;
    if (userInfo != "")
    {
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var info = parser.parse("{" + userInfo);
        Poco::JSON::Object::Ptr infoJson = info.extract<Poco::JSON::Object::Ptr>();
        role = infoJson->getValue<string>("role");
    }
    if (role != "a")
    {
        int count;
        jsonStr = MySQLHandler::getHandler()->getStudentSchedule(id, count);
        if (count == 0) jsonStr = "{ \"status\" : \"success\" }";
    }
    std::cout << jsonStr << std::endl;
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(jsonStr);
    ostream& out = resp.send();
    out << result.toString();
}

bool RequestHandler::CheckTokenAndRole(Poco::Net::HTTPServerRequest& req, std::string& jsonStr, std::string& role, Poco::JSON::Object::Ptr& ptr)
{
    Poco::JSON::Object::Ptr object = parseObjectJson(getRequestString(req));
    ptr = object;

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