#include "Session.h"


Session::Session()
{
	createdTime = std::chrono::system_clock::now();

	std::time_t t = std::chrono::system_clock::to_time_t(createdTime);

	//token is createdTime converted to string and encrypted as md5
	Poco::MD5Engine md5;
	Poco::DigestOutputStream ds(md5);
	ds << std::ctime(&t);
	ds.close();
	token =  Poco::DigestEngine::digestToHex(md5.digest());
}