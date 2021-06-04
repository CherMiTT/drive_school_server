#pragma once

#include "Poco/Data/Session.h"
#include "Poco/Data/MySQL/Connector.h"
#include "Poco/Data/MySQL/MySQLException.h"
#include <mutex>
#include "Session.h"

//Singleton
class MySQLHandler
{

public:
	[[nodiscard]] static MySQLHandler* getHandler();
	//[[nodiscard]] bool createConnection();

	[[nodiscard]] int getUserID(std::string, std::string);
	[[nodiscard]] struct User getUserInfo(Session*);

	void addUser(User newUser, std::string login, std::string password, 
		std::string phone, std::string email, std::string pass);
	 
private:
	static MySQLHandler *instance;
	static std::mutex mutex_;

	MySQLHandler() {}
};

struct User
{
	std::string first_name;
	std::string middle_name;
	std::string last_name;
	std::string role;
};

struct Student
{
	unsigned int group_id;
};
