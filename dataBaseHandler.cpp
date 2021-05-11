#include "dataBaseHandler.h"

bool MySQLHandler::createConnection()
{
	//register MySQL connector
	Poco::Data::MySQL::Connector::registerConnector();

    try
    {
        Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
        session = s;
        std::cout << "Connected to database successfully" << std::endl;
        return true;
    }
    catch (Poco::Data::MySQL::MySQLException& e)
    {
        std::cout << "Error: " << e.displayText() << std::endl;
        return false;
    }
    catch (Poco::Data::ConnectionFailedException& e)
    {
        std::cout << "Error: " << e.displayText() << std::endl;
        return false;
    }

    return false;
}
