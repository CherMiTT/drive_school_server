#include "server.h"
#include "RequestHandler.h"
#include "dataBaseHandler.h"

std::vector<Session*>* Server::vSessions = nullptr;


int Server::main(const vector<string>&)
{
	Poco::Net::HTTPServer s(new RequestHandlerFactory, Poco::Net::ServerSocket(9090), new Poco::Net::HTTPServerParams);
	s.start();
	std::cout << endl << "Server started" << std::endl;
	vSessions = new std::vector<Session*>();

	waitForTerminationRequest();  // wait for CTRL-C or kill

	std::cout << std::endl << "Shutting down..." << std::endl;
	s.stop();

	return Application::EXIT_OK;
}
