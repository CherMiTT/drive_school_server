#pragma once

#include <tuple>
#include <vector>
#include <chrono>

#include "Poco/MD5Engine.h"
#include "Poco/DigestStream.h"

class Session
{
public:
	int id;
	std::string token;
	std::chrono::time_point<std::chrono::system_clock> createdTime;

	Session();
};