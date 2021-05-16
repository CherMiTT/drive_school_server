#pragma once

#include "Poco/Data/Session.h"
#include "Poco/Data/MySQL/Connector.h"
#include "Poco/Data/MySQL/MySQLException.h"
#include <mutex>

//Singleton
class MySQLHandler
{

public:
	[[nodiscard]] static MySQLHandler* getHandler();
	//[[nodiscard]] bool createConnection();

	[[nodiscard]] int getUserID(std::string, std::string);
	[[nodiscard]] void getProfileInfo(Session*);
	 
private:
	//Poco::Data::Session *session;
	static MySQLHandler *instance;
	static std::mutex mutex_;

	MySQLHandler() {}
};

struct user
{
	std::string first_name;
	std::string middle_name;
	std::string last_name;
	std::string role;
};

struct student
{
	unsigned int group_id;
};
