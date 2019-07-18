#pragma once
#include <libssh2.h>
#include <libssh2_sftp.h>
#include <string>
#include "SSHChannel.h"
#include <iostream>
#include "../../Utility/DynamicLibExprotMarco.h"
using std::string;
class  CSSHshell
{
public:
	CSSHshell(const string &srvIp, int srvPort = 22);
	virtual ~CSSHshell();
	bool Connect(const string &userName, const string &userPwd);
	bool Disconnect(void);
	Channel* CreateChannel(const string &ptyType = "vanilla");
public:
	static void S_KbdCallback(const char*, int, const char*, int, int, const LIBSSH2_USERAUTH_KBDINT_PROMPT*, LIBSSH2_USERAUTH_KBDINT_RESPONSE*, void **a);
	static string s_password;

private:
	string m_srvIp				= {};
	int    m_srvPort			= {0};
	string m_userName			= {};
	string m_password			= {};
#ifdef _WIN32
	SOCKET m_sock				= { INVALID_SOCKET };
#else
	int   m_sock				= { 0 };
#endif // _WIN32
	LIBSSH2_SESSION *m_session	= {nullptr};
};


