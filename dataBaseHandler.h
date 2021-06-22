#pragma once

#include "Poco/Data/Session.h"
#include "Poco/Data/MySQL/Connector.h"
#include "Poco/Data/MySQL/MySQLException.h"
#include "Poco/StringTokenizer.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"
#include <mutex>
#include "Session.h"

//Singleton
class MySQLHandler
{

public:
	[[nodiscard]] static MySQLHandler* getHandler();

	[[nodiscard]] int getUserID(std::string, std::string);
	[[nodiscard]] struct User getUserInfo(Session*);
	[[nodiscard]] std::string getAllUsers(int& count);
	[[nodiscard]] std::string getAdmins(int& count);
	[[nodiscard]] std::string getInstructors(int& count);
	[[nodiscard]] std::string getStudents(int& count);
	[[nodiscard]] std::string getGroups(int& count);
	[[nodiscard]] std::string getCars(int& count);
	[[nodiscard]] std::string getRooms(int& count);
	[[nodiscard]] std::string getAllLessons(int& count);
	[[nodiscard]] std::string getAllTests(int& count);

	void addUser(User newUser);
	void addStudent(User newUser, std::string group);
	void addInstructor(User newUser, int salary);
	void addCar(std::string model, std::string plate, int status);
	void addRoom(int room);
	void addGroup(std::string name);
	void addLesson(Poco::JSON::Object::Ptr ptr);
	void addTest(Poco::JSON::Object::Ptr ptr, int author_id);
	void deleteUser(std::string login);
	void deleteGroup(int group);
	void deleteRoom(int room);
	void deleteCar(std::string login);
	void deleteLesson(int id);
	void deleteTest(int id);

private:
	static MySQLHandler *instance;
	static std::mutex mutex_;

	MySQLHandler() {}
};

struct User
{
	std::string first_name;
	std::string middle_name;
	std::string last_name;
	std::string login;
	std::string password;
	std::string phone_number;
	std::string email;
	std::string phone;
	std::string registration_date;
	std::string pass_number;
	std::string role;
};

struct Student
{
	unsigned int group_id;
};
