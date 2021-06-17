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

std::string MySQLHandler::getAllUsers(int& count)
{
    Poco::Data::MySQL::Connector::registerConnector();
    Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
    Poco::Data::Statement select(s);
    std::string result = "[";
    std::vector<std::string> first_name;
    std::vector<std::string> middle_name;
    std::vector<std::string> last_name;
    std::vector<std::string> login;
    std::vector<std::string> phone_number;
    std::vector<std::string> email;
    std::vector<std::string> pass_number;
    std::vector<Poco::DateTime> registration_date;
    std::vector<std::string> role;

    count = 0;

    select << "SELECT first_name, middle_name, last_name, login, phone_number, email, pass_number, registration_date, role FROM users",
        Poco::Data::Keywords::into(first_name),
        Poco::Data::Keywords::into(middle_name),
        Poco::Data::Keywords::into(last_name),
        Poco::Data::Keywords::into(login),
        Poco::Data::Keywords::into(phone_number),
        Poco::Data::Keywords::into(email),
        Poco::Data::Keywords::into(pass_number),
        Poco::Data::Keywords::into(registration_date),
        Poco::Data::Keywords::into(role);
    
    try
    {
        count = select.execute();
    }
    catch (const Poco::Exception& exc)
    {
            std::cout << "Exception while selecting all users:" << exc.name() << " : " << exc.displayText() << "; count = " << count << std::endl;
    }
    s.close();

    for (int i = 0; i < count; i++)
    {
        result += " {\"login\" : \"" + login[i] + "\", \"first_name\" : \"" +
            first_name[i] + "\", \"middle_name\" : \"" + middle_name[i] + "\", \"last_name\" : \"" +
            last_name[i] + "\", \"phone_number\" : \"" + phone_number[i] + "\", \"email\" : \"" +
            email[i] + "\", \"pass_number\" : \"" + pass_number[i] + "\", \"registration_date\" : \"" +
            Poco::DateTimeFormatter::format(registration_date[i], "%d %m %Y") + "\", \"role\" : \"" + role[i] + "\"},";
    }
    if (count != 0) result.pop_back();
    return result + "]";
}

std::string MySQLHandler::getInstructors(int& count)
{
    Poco::Data::MySQL::Connector::registerConnector();
    Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
    Poco::Data::Statement select(s);
    std::string result = "[";
    std::vector<std::string> first_name;
    std::vector<std::string> middle_name;
    std::vector<std::string> last_name;
    std::vector<std::string> login;
    std::vector<std::string> phone_number;
    std::vector<std::string> email;
    std::vector<Poco::DateTime> hired_date;
    std::vector<int> salary;

    count = 0;

    select << "SELECT first_name, middle_name, last_name, login, phone_number, email, hired_date, salary FROM users JOIN instructors WHERE role = \"i\"",
        Poco::Data::Keywords::into(first_name),
        Poco::Data::Keywords::into(middle_name),
        Poco::Data::Keywords::into(last_name),
        Poco::Data::Keywords::into(login),
        Poco::Data::Keywords::into(phone_number),
        Poco::Data::Keywords::into(email),
        Poco::Data::Keywords::into(hired_date),
        Poco::Data::Keywords::into(salary);

    try
    {
        count = select.execute();
    }
    catch (const Poco::Exception& exc)
    {
        std::cout << "Exception while selecting instructors:" << exc.name() << " : " << exc.displayText() << "; count = " << count << std::endl;
    }
    s.close();

    for (int i = 0; i < count; i++)
    {
        result += " {\"login\" : \"" + login[i] + "\", \"first_name\" : \"" +
            first_name[i] + "\", \"middle_name\" : \"" + middle_name[i] + "\", \"last_name\" : \"" +
            last_name[i] + "\", \"phone_number\" : \"" + phone_number[i] + "\", \"email\" : \"" +
            email[i] + "\", \"hired_date\" : \"" + Poco::DateTimeFormatter::format(hired_date[i], "%d %m %Y") + "\", \"salary\" : " + std::to_string(salary[i]) + "},";
    }
    if (count != 0) result.pop_back();
    return result + "]";
}

std::string MySQLHandler::getStudents(int& count)
{
    Poco::Data::MySQL::Connector::registerConnector();
    Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
    Poco::Data::Statement select(s);
    std::string result = "[";
    std::vector<std::string> first_name;
    std::vector<std::string> middle_name;
    std::vector<std::string> last_name;
    std::vector<std::string> login;
    std::vector<std::string> phone_number;
    std::vector<std::string> email;
    std::vector<std::string> group_name;

    count = 0;

    select << "SELECT u.first_name, u.middle_name, u.last_name, u.login, u.phone_number, u.email, g.group_name FROM users u JOIN students s ON (u.id = s.users_id) JOIN study_groups g ON(s.groups_id = g.id) WHERE role = \"s\"",
        Poco::Data::Keywords::into(first_name),
        Poco::Data::Keywords::into(middle_name),
        Poco::Data::Keywords::into(last_name),
        Poco::Data::Keywords::into(login),
        Poco::Data::Keywords::into(phone_number),
        Poco::Data::Keywords::into(email),
        Poco::Data::Keywords::into(group_name);

    try
    {
        count = select.execute();
    }
    catch (const Poco::Exception& exc)
    {
        std::cout << "Exception while selecting students:" << exc.name() << " : " << exc.displayText() << "; count = " << count << std::endl;
    }
    s.close();

    for (int i = 0; i < count; i++)
    {
        result += " {\"login\" : \"" + login[i] + "\", \"first_name\" : \"" +
            first_name[i] + "\", \"middle_name\" : \"" + middle_name[i] + "\", \"last_name\" : \"" +
            last_name[i] + "\", \"phone_number\" : \"" + phone_number[i] + "\", \"email\" : \"" +
            email[i] + "\", \"group\" : \"" + group_name[i] + "\"},";
    }
    if(count != 0) result.pop_back();
    return result + "]";
}

std::string MySQLHandler::getGroups(int& count)
{
    Poco::Data::MySQL::Connector::registerConnector();
    Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
    Poco::Data::Statement select(s);
    std::string result = "[";
    std::vector<int> id;
    std::vector<std::string> group;

    count = 0;

    select << "SELECT id, group_name FROM study_groups",
        Poco::Data::Keywords::into(id),
        Poco::Data::Keywords::into(group);

    try
    {
        count = select.execute();
    }
    catch (const Poco::Exception& exc)
    {
        std::cout << "Exception while selecting all groups:" << exc.name() << " : " << exc.displayText() << "; count = " << count << std::endl;
    }
    s.close();

    for (int i = 0; i < count; i++)
    {
        result += " {\"id\" : \"" + std::to_string(id[i]) + "\", \"group\" : \"" + group[i] + "\"},";
    }
    if (count != 0) result.pop_back();
    return result + "]";
}

std::string MySQLHandler::getCars(int& count)
{
    Poco::Data::MySQL::Connector::registerConnector();
    Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
    Poco::Data::Statement select(s);
    std::string result = "[";
    std::vector<std::string> plate;
    std::vector<std::string> model;
    std::vector<std::string> status;

    count = 0;

    select << "SELECT plate, model, status FROM cars",
        Poco::Data::Keywords::into(plate),
        Poco::Data::Keywords::into(status),
        Poco::Data::Keywords::into(model);

    try
    {
        count = select.execute();
    }
    catch (const Poco::Exception& exc)
    {
        std::cout << "Exception while selecting all cars:" << exc.name() << " : " << exc.displayText() << "; count = " << count << std::endl;
    }
    s.close();

    for (int i = 0; i < count; i++)
    {
        result += " {\"plate\" : \"" + plate[i] + "\", \"model\" : \"" + model[i] + "\", \"status\" : \"" + status[i] +"\"},";
    }
    if (count != 0) result.pop_back();
    return result + "]";
}

std::string MySQLHandler::getRooms(int& count)
{
    Poco::Data::MySQL::Connector::registerConnector();
    Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
    Poco::Data::Statement select(s);
    std::string result = "[";
    std::vector<int> number;
    std::vector<std::string> group;

    count = 0;

    select << "SELECT number FROM auditories",
        Poco::Data::Keywords::into(number);

    try
    {
        count = select.execute();
    }
    catch (const Poco::Exception& exc)
    {
        std::cout << "Exception while selecting all users:" << exc.name() << " : " << exc.displayText() << "; count = " << count << std::endl;
    }
    s.close();

    for (int i = 0; i < count; i++)
    {
        result += " {\"number\" : \"" + std::to_string(number[i]) + "\"},";
    }
    if (count != 0) result.pop_back();
    return result + "]";
}

std::string MySQLHandler::getAdmins(int& count)
{
    Poco::Data::MySQL::Connector::registerConnector();
    Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
    Poco::Data::Statement select(s);
    std::string result = "[";
    std::vector<std::string> first_name;
    std::vector<std::string> middle_name;
    std::vector<std::string> last_name;
    std::vector<std::string> login;
    std::vector<std::string> phone_number;
    std::vector<std::string> email;

    count = 0;

    select << "SELECT first_name, middle_name, last_name, login, phone_number, email FROM users WHERE role = \"a\"",
        Poco::Data::Keywords::into(first_name),
        Poco::Data::Keywords::into(middle_name),
        Poco::Data::Keywords::into(last_name),
        Poco::Data::Keywords::into(login),
        Poco::Data::Keywords::into(phone_number),
        Poco::Data::Keywords::into(email);

    try
    {
        count = select.execute();
    }
    catch (const Poco::Exception& exc)
    {
        std::cout << "Exception while selecting admins:" << exc.name() << " : " << exc.displayText() << "; count = " << count << std::endl;
    }
    s.close();

    for (int i = 0; i < count; i++)
    {
        result += " {\"login\" : \"" + login[i] + "\", \"first_name\" : \"" +
            first_name[i] + "\", \"middle_name\" : \"" + middle_name[i] + "\", \"last_name\" : \"" +
            last_name[i] + "\", \"phone_number\" : \"" + phone_number[i] + "\", \"email\" : \"" +
            email[i] + "\"},";
    }
    if (count != 0) result.pop_back();
    return result + "]";
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