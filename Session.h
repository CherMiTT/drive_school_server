#pragma once

#include <tuple>
#include <vector>
#include <chrono>

#include "Poco/MD5Engine.h"
#include "Poco/DigestStream.h"

class Session
{
public:
	int id; //TODO: id into user struct
	std::string token;
	std::chrono::time_point<std::chrono::system_clock> createdTime;
	std::string userInfoJsonString; //TODO: store struct, not string
	Session();
};