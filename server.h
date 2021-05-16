#pragma once

#include <iostream>
#include <vector>
#include <conio.h>
#include <string>

#include <Poco/Util/ServerApplication.h>
#include <Poco/Mutex.h>

#include "Session.h"
using namespace Poco;
using namespace Poco::Util;
using namespace std;

class Server : public ServerApplication
{
public:
	static std::vector<Session*>* vSessions;

private:
	int main(const vector<string>&);

};
