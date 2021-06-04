#include "dataBaseHandler.h"

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

struct User MySQLHandler::getUserInfo(Session* session)
{
    struct User user;

    Poco::Data::MySQL::Connector::registerConnector();
    Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
    Poco::Data::Statement select(s);

    select << "SELECT first_name, middle_name, last_name, role FROM users WHERE id = ?",
        Poco::Data::Keywords::into(user.first_name),
        Poco::Data::Keywords::into(user.middle_name),
        Poco::Data::Keywords::into(user.last_name),
        Poco::Data::Keywords::into(user.role),
        Poco::Data::Keywords::use(session->id),
        Poco::Data::Keywords::now;
    s.close();

    return user;
}

void MySQLHandler::addUser(struct User newUser, std::string login, std::string password,
                            std::string phone, std::string email, std::string pass)
{
    Poco::Data::MySQL::Connector::registerConnector();
    Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
    Poco::Data::Statement insert(s);

    insert << "INSERT INTO `drive_school`.`users`(`login`,`password_hash`,`first_name`,`middle_name`,`last_name`,`phone_number`,`email`,`pass_number`,`role`) VALUES(?,?,?,?,?,?,?,?,?);",
        Poco::Data::Keywords::use(login), Poco::Data::Keywords::use(password),
        Poco::Data::Keywords::use(newUser.first_name), Poco::Data::Keywords::use(newUser.middle_name),
        Poco::Data::Keywords::use(newUser.last_name), Poco::Data::Keywords::use(phone),
        Poco::Data::Keywords::use(email), Poco::Data::Keywords::use(pass),
        Poco::Data::Keywords::use(newUser.role);
    try
    {
        insert.execute();
    }
    catch (const Poco::Exception& exc)
    {
        std::cout << "Exception while adding user:" << exc.displayText() << std::endl;
    }
    s.close();

}