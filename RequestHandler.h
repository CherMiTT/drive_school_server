#pragma once

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/ScopedLock.h>
#include <Poco/URI.h>
#include <Poco/StringTokenizer.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/Dynamic/Var.h>
#include <chrono>

#include "dataBaseHandler.h"

class RequestHandler : public Poco::Net::HTTPRequestHandler
{
public:
	void handleRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
	void handleAuthorizationRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
	void handleProfileInfoRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
	void handleAddUserRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
	void handleAllUsersRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
	void handleAdminsRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
	void handleInstructorsRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
	void handleStudentsRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
	void handleGroupsRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
	void handleCarsRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
	void handleRoomsRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
	void handleAddCarRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
	void handleAddRoomRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
	void handleAddGroupRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);

private:
	[[nodiscard]] Poco::JSON::Object::Ptr parseObjectJson(std::string&);
	[[nodiscard]] std::string getRequestString(Poco::Net::HTTPServerRequest&);
	[[nodiscard]] bool CheckTokenAndRole(Poco::Net::HTTPServerRequest& req, std::string& jsonStr, std::string& role, Poco::JSON::Object::Ptr& ptr);
};


class RequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
	[[nodiscard]] virtual Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest&);

};