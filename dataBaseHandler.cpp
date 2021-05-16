#include "dataBaseHandler.h"
#include "Session.h"

MySQLHandler* MySQLHandler::instance{nullptr};
std::mutex MySQLHandler::mutex_;


MySQLHandler* MySQLHandler::getHandler()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (instance == nullptr)
    {
        instance = new MySQLHandler();
    }
    return instance;
}


/*bool MySQLHandler::createConnection()
{
	//register MySQL connector
	Poco::Data::MySQL::Connector::registerConnector();

    try
    {
        Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
        this->session = &s;
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
}*/

int MySQLHandler::getUserID(std::string login, std::string password)
{
    int id = -1;

    Poco::Data::MySQL::Connector::registerConnector();
    Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
    Poco::Data::Statement select(s);

    select << "SELECT id FROM users WHERE login = ? AND password_hash = ?",
        Poco::Data::Keywords::into(id),
        Poco::Data::Keywords::use(login),
        Poco::Data::Keywords::use(password), 
        Poco::Data::Keywords::now;
    s.close();
    return id;
}

void MySQLHandler::getProfileInfo(Session *session)
{

}