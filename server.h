#pragma once

#include <iostream>
#include <vector>
#include <conio.h>
#include <string>

#include <Poco/Util/ServerApplication.h>
#include <Poco/Mutex.h>

using namespace Poco;
using namespace Poco::Util;
using namespace std;

class Server : public ServerApplication
{
public:
	static string getText();
	static void setText(string newText);

protected:
	int main(const vector<string>&);

	static string text;
	static Mutex textLock;

};