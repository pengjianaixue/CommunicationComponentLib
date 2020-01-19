#pragma once
#include <curl\curl.h>
#include <Windows.h>
#include <string>
#include <thread>
class CTelnet
{
public:
	CTelnet(const std::string &address, const std::string &port,const std::string &userAccount = "", const std::string &passwd="");
	~CTelnet();
//API
	bool connect();
	bool disconnect();
	bool send(const std::string &cmd);
	std::string recvi();
	void resetConnectInfor(const std::string &address, const std::string &port, const std::string &userAccount = "", const std::string &passwd = "");
private:
	void setConnectInfor(const std::string &address, const std::string &port, const std::string &userAccount = "", const std::string &passwd = "");
	bool login();
	CURL *m_curl{nullptr};
	struct curl_slist *m_options{nullptr};
	std::string m_address;
	std::string m_port;
	std::string m_userAccount;
	std::string m_passwd;


};

