#pragma once
#include <libssh2.h>
#include <libssh2_sftp.h>
#include <string>
#include <iostream>
using std::string;
class Channel
{
public:
	Channel(LIBSSH2_CHANNEL *channel);
	~Channel(void);

	string Read(const string &strend = "$", int timeout = 3000);
	bool   Write(const string &data);
private:
	Channel(const Channel&);
	Channel operator=(const Channel&);
private:
	LIBSSH2_CHANNEL *m_channel;
};

