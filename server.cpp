#include "Server.h"
#include "RequestHandler.h"
#include "dataBaseHandler.h"

string Server::text = "Hello world!";
Mutex Server::textLock;


string Server::getText()
{
	ScopedLock<Mutex> lock(textLock);
	return text;
}

void Server::setText(string newText)
{
	ScopedLock<Mutex> lock(textLock);
	text = newText;
}

int Server::main(const vector<string>&)
{
	Poco::Net::HTTPServer s(new RequestHandlerFactory, Poco::Net::ServerSocket(9090), new Poco::Net::HTTPServerParams);
	s.start();
	std::cout << endl << "Server started" << std::endl;

	bool flag = MySQLHandler::createConnection();
	if (!flag)
	{
		std::cout << "Couldn't connect to database!" << std::endl;
	}
	waitForTerminationRequest();  // wait for CTRL-C or kill

	std::cout << std::endl << "Shutting down..." << std::endl;
	s.stop();

	return Application::EXIT_OK;
}
