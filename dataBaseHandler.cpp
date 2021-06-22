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

std::string MySQLHandler::getAllLessons(int& count)
{
    Poco::Data::MySQL::Connector::registerConnector();
    Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
    Poco::Data::Statement select(s);
    std::string result = "[";
    std::vector<std::string> instructor_name;
    std::vector<std::string> group_name;
    std::vector<int> id;
    std::vector<int> aud_number;
    std::vector<Poco::DateTime> lesson_time;
    std::vector<std::string> lesson_type_name;

    count = 0;

    select << "SELECT instructor_name, group_name, id, aud_number, lesson_time, lesson_type_name FROM lessons_view",
        Poco::Data::Keywords::into(instructor_name),
        Poco::Data::Keywords::into(group_name),
        Poco::Data::Keywords::into(id),
        Poco::Data::Keywords::into(aud_number),
        Poco::Data::Keywords::into(lesson_time),
        Poco::Data::Keywords::into(lesson_type_name);

    try
    {
        count = select.execute();
    }
    catch (const Poco::Exception& exc)
    {
        std::cout << "Exception while selecting lessons:" << exc.name() << " : " << exc.displayText() << "; count = " << count << std::endl;
    }
    s.close();

    for (int i = 0; i < count; i++)
    {
        result += " {\"group_name\" : \"" + group_name[i] + "\", \"instructor_name\" : \"" +
            instructor_name[i] + "\", \"id\" : \"" + std::to_string(id[i]) + "\", \"aud_number\" : \"" +
            std::to_string(aud_number[i]) + "\", \"lesson_type_name\" : \"" + lesson_type_name[i] + "\", \"lesson_time\" : \"" +
            Poco::DateTimeFormatter::format(lesson_time[i], "%M:%H %d.%m.%Y") + "\"},";
    }
    if (count != 0) result.pop_back();
    return result + "]";
}

std::string MySQLHandler::getAllTests(int& count)
{
    Poco::Data::MySQL::Connector::registerConnector();
    Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
    Poco::Data::Statement select(s);
    std::string result = "[";
    std::vector<int> id;
    std::vector<int> author_id;
    std::vector<std::string> text;
    std::vector<std::string> var1;
    std::vector<std::string> var2;
    std::vector<std::string> var3;
    std::vector<std::string> var4;
    std::vector<int> answer;
    std::vector<std::string> comment;

    count = 0;

    select << "SELECT id, author_id, text, var1, var2, var3, var4, true_var_number, answer_comment FROM tests",
        Poco::Data::Keywords::into(id),
        Poco::Data::Keywords::into(author_id),
        Poco::Data::Keywords::into(text),
        Poco::Data::Keywords::into(var1),
        Poco::Data::Keywords::into(var2),
        Poco::Data::Keywords::into(var3),
        Poco::Data::Keywords::into(var4),
        Poco::Data::Keywords::into(answer),
        Poco::Data::Keywords::into(comment);

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
        result += " {\"id\" : \"" + std::to_string(id[i]) + "\", \"author_id\" : \"" +
            std::to_string(author_id[i]) + "\", \"text\" : \"" + text[i] + "\", \"var1\" : \"" +
            var1[i] + "\", \"var2\" : \"" + var2[i] + "\", \"var3\" : \"" +
            var3[i] + "\", \"var4\" : \"" + var4[i] + "\", \"answer\" : \"" 
            + std::to_string(answer[i]) + "\", \"comment\" : \"" + comment[i] +"\"},";
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

void MySQLHandler::addUser(struct User newUser)
{
    Poco::Data::MySQL::Connector::registerConnector();
    Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
    Poco::Data::Statement insert(s);

    insert << "INSERT INTO `drive_school`.`users`(`login`,`password_hash`,`first_name`,`middle_name`,`last_name`,`phone_number`,`email`,`pass_number`,`role`) VALUES(?,?,?,?,?,?,?,?,?);",
        Poco::Data::Keywords::use(newUser.login), Poco::Data::Keywords::use(newUser.password),
        Poco::Data::Keywords::use(newUser.first_name), Poco::Data::Keywords::use(newUser.middle_name),
        Poco::Data::Keywords::use(newUser.last_name), Poco::Data::Keywords::use(newUser.phone),
        Poco::Data::Keywords::use(newUser.email), Poco::Data::Keywords::use(newUser.pass_number),
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

void MySQLHandler::addStudent(User newUser, std::string group)
{
    Poco::Data::MySQL::Connector::registerConnector();
    Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
    Poco::Data::Statement selectStudentId(s);
    Poco::Data::Statement selectGroupId(s);
    Poco::Data::Statement insert(s);

    int id;
    int groupId;
    selectStudentId << "SELECT id FROM users WHERE login = ?",
        Poco::Data::Keywords::into(id),
        Poco::Data::Keywords::use(newUser.login),
        Poco::Data::Keywords::now;

    selectGroupId << "SELECT id FROM study_groups WHERE group_name = ?",
        Poco::Data::Keywords::into(groupId),
        Poco::Data::Keywords::use(group),
        Poco::Data::Keywords::now;

    insert << "INSERT INTO `drive_school`.`students` (`users_id`, `groups_id`) VALUES(?,?);",
        Poco::Data::Keywords::use(id), Poco::Data::Keywords::use(groupId);
    try
    {
        insert.execute();
    }
    catch (const Poco::Exception& exc)
    {
        std::cout << "Exception while adding student:" << exc.displayText() << std::endl;
    }
    s.close();

}

void MySQLHandler::addInstructor(User newUser, int salary)
{
    Poco::Data::MySQL::Connector::registerConnector();
    Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
    Poco::Data::Statement selectUserId(s);
    Poco::Data::Statement insert(s);

    int id;
    selectUserId << "SELECT id FROM users WHERE login = ?",
        Poco::Data::Keywords::into(id),
        Poco::Data::Keywords::use(newUser.login),
        Poco::Data::Keywords::now;

    insert << "INSERT INTO instructors (users_id, salary) VALUES(?,?);",
        Poco::Data::Keywords::use(id), Poco::Data::Keywords::use(salary);
    try
    {
        insert.execute();
    }
    catch (const Poco::Exception& exc)
    {
        std::cout << "Exception while adding instructor:" << exc.displayText() << std::endl;
    }
    s.close();
}

void MySQLHandler::addCar(std::string model, std::string plate, int status)
{
    Poco::Data::MySQL::Connector::registerConnector();
    Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
    Poco::Data::Statement insert(s);
    
    std::string stat = status == 0 ? "active" : "inactive";
    insert << "INSERT INTO cars (model, plate, status) VALUES(?,?,?);",
        Poco::Data::Keywords::use(model), Poco::Data::Keywords::use(plate),
        Poco::Data::Keywords::use(stat);
    try
    {
        insert.execute();
    }
    catch (const Poco::Exception& exc)
    {
        std::cout << "Exception while adding car:" << exc.displayText() << std::endl;
    }
    s.close();
}

void MySQLHandler::addRoom(int room)
{
    Poco::Data::MySQL::Connector::registerConnector();
    Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
    Poco::Data::Statement insert(s);

    insert << "INSERT INTO auditories (number) VALUES(?);",
        Poco::Data::Keywords::use(room);
    try
    {
        insert.execute();
    }
    catch (const Poco::Exception& exc)
    {
        std::cout << "Exception while adding room:" << exc.displayText() << std::endl;
    }
    s.close();
}

void MySQLHandler::addGroup(std::string name)
{
    Poco::Data::MySQL::Connector::registerConnector();
    Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
    Poco::Data::Statement insert(s);

    insert << "INSERT INTO study_groups (group_name) VALUES(?);",
        Poco::Data::Keywords::use(name);
    try
    {
        insert.execute();
    }
    catch (const Poco::Exception& exc)
    {
        std::cout << "Exception while adding group:" << exc.displayText() << std::endl;
    }
    s.close();
}

void MySQLHandler::addLesson(Poco::JSON::Object::Ptr ptr)
{
    Poco::Data::MySQL::Connector::registerConnector();
    Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
    Poco::Data::Statement selectGroupId(s);
    Poco::Data::Statement selectInstructorId(s);
    Poco::Data::Statement insert(s);

    int groupId;
    selectGroupId << "SELECT id FROM study_groups WHERE group_name = ?",
        Poco::Data::Keywords::into(groupId),
        Poco::Data::Keywords::use(ptr->getValue<std::string>("group")),
        Poco::Data::Keywords::now;
    
    int instructorId;
    std::string instr = ptr->getValue<std::string>("instructor");
    instr = instr.substr(0, instr.find(" "));

    selectInstructorId << "SELECT id FROM users WHERE first_name = ?",
        Poco::Data::Keywords::into(instructorId),
        Poco::Data::Keywords::use(instr),
        Poco::Data::Keywords::now;

    int type_id = ptr->getValue<int>("type");
    int room = ptr->getValue<int>("room");
    std::string timestr = ptr->getValue<std::string>("time");
    std::string formatstr = "%Y %m %d %H %M %S";
    int timeZoneDifferential = 0;
    Poco::DateTime time = Poco::DateTimeParser::parse(formatstr, timestr, timeZoneDifferential);
    insert << "INSERT INTO lessons (groups_id, instructors_users_id, lesson_types_id, auditories_number, time) VALUES(?,?,?,?,?);",
        Poco::Data::Keywords::use(groupId),
        Poco::Data::Keywords::use(instructorId),
        Poco::Data::Keywords::use(type_id),
        Poco::Data::Keywords::use(room),
        Poco::Data::Keywords::use(time);
    
    try
    {
        insert.execute();
    }
    catch (const Poco::Exception& exc)
    {
        std::cout << "Exception while adding instructor:" << exc.displayText() << std::endl;
    }
    s.close();
}

void MySQLHandler::addTest(Poco::JSON::Object::Ptr ptr, int author_id)
{
    Poco::Data::MySQL::Connector::registerConnector();
    Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
    Poco::Data::Statement insert(s);
    int answer = ptr->getValue<int>("answer");
    std::string image = ptr->getValue<std::string>("image");
    std::string text = ptr->getValue<std::string>("text");
    std::string var1 = ptr->getValue<std::string>("var1");
    std::string var2 = ptr->getValue<std::string>("var2");
    std::string var3 = ptr->getValue<std::string>("var3");
    std::string var4 = ptr->getValue<std::string>("var4");
    std::string comment = ptr->getValue<std::string>("comment");


    insert << "INSERT INTO tests (author_id, image, text, var1, var2, var3, var4, true_var_number, answer_comment) VALUES(?,?,?,?,?,?,?,?,?);",
        Poco::Data::Keywords::use(author_id),
        Poco::Data::Keywords::use(image),
        Poco::Data::Keywords::use(text),
        Poco::Data::Keywords::use(var1),
        Poco::Data::Keywords::use(var2),
        Poco::Data::Keywords::use(var3),
        Poco::Data::Keywords::use(var4),
        Poco::Data::Keywords::use(answer),
        Poco::Data::Keywords::use(comment);

    try
    {
        insert.execute();
    }
    catch (const Poco::Exception& exc)
    {
        std::cout << "Exception while adding instructor:" << exc.displayText() << std::endl;
    }
    s.close();
}

void MySQLHandler::deleteUser(std::string login)
{
    Poco::Data::MySQL::Connector::registerConnector();
    Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
    Poco::Data::Statement d(s);

    d << "DELETE FROM users WHERE login = ?",
        Poco::Data::Keywords::use(login);
    try
    {
        d.execute();
    }
    catch (const Poco::Exception& exc)
    {
        std::cout << "Exception while deleting user:" << exc.displayText() << std::endl;
    }
    s.close();
}

void MySQLHandler::deleteGroup(int group)
{
    Poco::Data::MySQL::Connector::registerConnector();
    Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
    Poco::Data::Statement d(s);

    d << "DELETE FROM study_groups WHERE id = ?",
        Poco::Data::Keywords::use(group);
    try
    {
        d.execute();
    }
    catch (const Poco::Exception& exc)
    {
        std::cout << "Exception while deleting group:" << exc.displayText() << std::endl;
    }
    s.close();
}

void MySQLHandler::deleteRoom(int room)
{
    Poco::Data::MySQL::Connector::registerConnector();
    Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
    Poco::Data::Statement d(s);

    d << "DELETE FROM auditories WHERE number = ?",
        Poco::Data::Keywords::use(room);
    try
    {
        d.execute();
    }
    catch (const Poco::Exception& exc)
    {
        std::cout << "Exception while deleting room:" << exc.displayText() << std::endl;
    }
    s.close();
}

void MySQLHandler::deleteCar(std::string plate)
{
    Poco::Data::MySQL::Connector::registerConnector();
    Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
    Poco::Data::Statement d(s);

    d << "DELETE FROM cars WHERE plate = ?",
        Poco::Data::Keywords::use(plate);
    try
    {
        d.execute();
    }
    catch (const Poco::Exception& exc)
    {
        std::cout << "Exception while deleting car:" << exc.displayText() << std::endl;
    }
    s.close();
}

void MySQLHandler::deleteLesson(int id)
{
    Poco::Data::MySQL::Connector::registerConnector();
    Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
    Poco::Data::Statement d(s);

    d << "DELETE FROM lessons WHERE id = ?",
        Poco::Data::Keywords::use(id);
    try
    {
        d.execute();
    }
    catch (const Poco::Exception& exc)
    {
        std::cout << "Exception while deleting lesson:" << exc.displayText() << std::endl;
    }
    s.close();
}

void MySQLHandler::deleteTest(int id)
{
    Poco::Data::MySQL::Connector::registerConnector();
    Poco::Data::Session s("MySQL", "host=localhost;port=3306;db=drive_school;user=root;password=qwerty;compress=true;auto-reconnect=true");
    Poco::Data::Statement d(s);

    d << "DELETE FROM tests WHERE id = ?",
        Poco::Data::Keywords::use(id);
    try
    {
        d.execute();
    }
    catch (const Poco::Exception& exc)
    {
        std::cout << "Exception while deleting test:" << exc.displayText() << std::endl;
    }
    s.close();
}