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

#include "dataBaseHandler.h"

//using namespace Poco::Net;
//using namespace std;

class RequestHandler : public Poco::Net::HTTPRequestHandler
{
public:
	void handleRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
	void handleAuthorizationRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);
	void handleProfileInfoRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse&);

private:
	[[nodiscard]] Poco::JSON::Object::Ptr parseAuthorizationJson(std::string&);
	[[modiscard]] std::string getRequestString(Poco::Net::HTTPServerRequest&);
};


class RequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
	[[nodiscard]] virtual Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest&);

};