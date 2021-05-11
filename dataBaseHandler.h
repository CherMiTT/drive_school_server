#pragma once

#include "Poco/Data/Session.h"
#include "Poco/Data/MySQL/Connector.h"
#include "Poco/Data/MySQL/MySQLException.h"


class MySQLHandler
{
public:
	[[nodiscard]] static bool createConnection();

private:
	static Poco::Data::Session session;
};