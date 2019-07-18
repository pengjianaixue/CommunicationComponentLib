#include "SSHshell.h"
#pragma comment(lib,"libssh2.lib")
std::string CSSHshell::s_password = {};
CSSHshell::CSSHshell(const string &srvIp, int srvPort)
		:m_srvIp(srvIp),m_srvPort(srvPort)
{
	libssh2_init(0);
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	WSAStartup(sockVersion, &wsaData);

}


CSSHshell::~CSSHshell()
{
	Disconnect();
	libssh2_exit();
}

bool CSSHshell::Connect(const string & userName, const string & userPwd)
{
	m_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(m_srvIp.c_str());
	sin.sin_port = htons(m_srvPort);
	if (::connect(m_sock, (sockaddr*)(&sin), sizeof(sin))!=0)
	{
		int errorcode = WSAGetLastError();
		std::cout << errorcode << std::endl;
		Disconnect();
		return false;
	}

	m_session = libssh2_session_init();
	if (!m_session)
	{
		return false;
	}
	int rc = 0;
	if ((rc = libssh2_session_handshake(m_session, m_sock)))
	{
		fprintf(stderr, "Error when starting up SSH session: %d\n", rc);
		return -1;
		Disconnect();
		return false;
	}
	int auth_pw = 0;
	const char* fingerprint = libssh2_hostkey_hash(m_session, LIBSSH2_HOSTKEY_HASH_SHA1);
	fprintf(stderr, "Fingerprint: ");
	for (int i = 0; i < 20; i++) {

		fprintf(stderr, "%02X ", (unsigned char)fingerprint[i]);
	}
	fprintf(stderr, "\n");
	char* userauthlistchar = libssh2_userauth_list(m_session, userName.c_str(), userName.size());
	string userauthlist = userauthlistchar;
	if (strstr(userauthlist.c_str(), "password") != NULL)
	{
		auth_pw |= 1;
	}
	if (strstr(userauthlist.c_str(), "keyboard-interactive") != NULL)
	{
		auth_pw |= 2;
	}
	if (strstr(userauthlist.c_str(), "publickey") != NULL)
	{
		auth_pw |= 4;
	}

	if (auth_pw & 1)
	{
		/* We could authenticate via password */
		if (libssh2_userauth_password(m_session, userName.c_str(), userPwd.c_str()))
		{
			Disconnect();
			return false;
		}
	}
	else if (auth_pw & 2)
	{
		/* Or via keyboard-interactive */
		s_password = userPwd;
		if (libssh2_userauth_keyboard_interactive(m_session, userName.c_str(), &S_KbdCallback))
		{
			Disconnect();
			return false;
		}
	}
	else
	{
		Disconnect();
		return false;
	}

	return true;
}

bool CSSHshell::Disconnect(void)
{
	if (m_session)
	{
		libssh2_session_disconnect(m_session, "Bye bye, Thank you");
		libssh2_session_free(m_session);
		m_session = nullptr;
	}
	if (m_sock != -1)
	{
#ifdef _WIN32
		closesocket(m_sock);
#else
		close(m_sock);
#endif
		m_sock = INVALID_SOCKET;
	}
	return true;
}

Channel * CSSHshell::CreateChannel(const string & ptyType)
{

	if (NULL == m_session)
	{
		return NULL;
	}

	LIBSSH2_CHANNEL *channel = NULL;
	/* Request a shell */
	if ((channel = libssh2_channel_open_session(m_session)))
	{
		return NULL;
	}

	/* Request a terminal with 'vanilla' terminal emulation
	* See /etc/termcap for more options
	*/
	if (libssh2_channel_request_pty(channel, ptyType.c_str()))
	{
		libssh2_channel_free(channel);
		return NULL;
	}

	/* Open a SHELL on that pty */
	if (libssh2_channel_shell(channel))
	{

		libssh2_channel_free(channel);
		return NULL;
	}

	Channel *ret = new Channel(channel);
	std::cout << ret->Read() << std::endl;
	return ret;
}

void CSSHshell::S_KbdCallback(const char *name, int name_len,
								const char *instruction, int instruction_len,
								int num_prompts,
								const LIBSSH2_USERAUTH_KBDINT_PROMPT *prompts,
								LIBSSH2_USERAUTH_KBDINT_RESPONSE *responses,
								void **abstract)
{
	(void)name;
	(void)name_len;
	(void)instruction;
	(void)instruction_len;
	if (num_prompts == 1)
	{
		responses[0].text = const_cast<char*>(s_password.c_str());
		responses[0].length = (int)s_password.size();
	}
	(void)prompts;
	(void)abstract;
}
